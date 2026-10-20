import copy
import streamlit as st
import pandas as pd

from algorithms.bankers import calculate_need, safety_check, try_request
from algorithms.detection import detect_deadlock
from components.graph import draw_resource_allocation_graph


st.set_page_config(
    page_title="Deadlock Lab",
    page_icon="⚙️",
    layout="wide",
    initial_sidebar_state="expanded",
)

# ------------------------------- Scenarios --------------------------------
def safe_scenario():
    return {
        "totals": [10, 5, 7],
        "allocation": [
            [0, 1, 0],
            [2, 0, 0],
            [3, 0, 2],
            [2, 1, 1],
            [0, 0, 2],
        ],
        "maximum": [
            [7, 5, 3],
            [3, 2, 2],
            [9, 0, 2],
            [2, 2, 2],
            [4, 3, 3],
        ],
        "requests": [[0, 0, 0] for _ in range(5)],
        "events": ["Loaded the classic safe Banker’s Algorithm example."],
    }


def deadlock_scenario():
    return {
        "totals": [1, 1],
        "allocation": [[1, 0], [0, 1]],
        "maximum": [[1, 1], [1, 1]],
        "requests": [[0, 1], [1, 0]],
        "events": ["Loaded a deliberate two-process deadlock: P0 holds R0 and waits for R1; P1 holds R1 and waits for R0."],
    }


def initialize_state(scenario=None):
    data = scenario or safe_scenario()
    st.session_state["totals"] = copy.deepcopy(data["totals"])
    st.session_state["allocation"] = copy.deepcopy(data["allocation"])
    st.session_state["maximum"] = copy.deepcopy(data["maximum"])
    st.session_state["requests"] = copy.deepcopy(data["requests"])
    st.session_state["events"] = copy.deepcopy(data["events"])
    st.session_state["last_result"] = "Ready. Run a safety check or try a scenario."
    st.session_state["process_count_cfg"] = len(data["allocation"])
    st.session_state["resource_count_cfg"] = len(data["totals"])
    # Keep the sidebar widgets synchronized when a preset scenario is loaded.
    st.session_state["cfg_process_count"] = len(data["allocation"])
    st.session_state["cfg_resource_count"] = len(data["totals"])
    for key in ("allocation_editor", "maximum_editor", "requests_editor"):
        st.session_state.pop(key, None)
    for j in range(20):
        st.session_state.pop(f"total_{j}", None)
        st.session_state.pop(f"req_{j}", None)
        st.session_state.pop(f"release_{j}", None)


if "allocation" not in st.session_state:
    initialize_state()


def log_event(message):
    st.session_state["events"].append(message)
    st.session_state["last_result"] = message


def matrix_df(matrix, prefix="P"):
    return pd.DataFrame(
        matrix,
        index=[f"{prefix}{i}" for i in range(len(matrix))],
        columns=[f"R{j}" for j in range(len(matrix[0]) if matrix else 0)],
    )


def read_editor_matrix(df):
    # Convert dataframe editor values to integers; reject blanks/non-numeric values.
    result = []
    for row in df.itertuples(index=False, name=None):
        converted = []
        for value in row:
            if pd.isna(value):
                raise ValueError("Matrix cells cannot be blank.")
            numeric = float(value)
            if not numeric.is_integer():
                raise ValueError("Matrix values must be whole numbers.")
            if numeric < 0:
                raise ValueError("Matrix values cannot be negative.")
            converted.append(int(numeric))
        result.append(converted)
    return result


def validate_state(allocation, maximum, totals, requests):
    n = len(allocation)
    m = len(totals)
    if len(maximum) != n or len(requests) != n:
        return "Allocation, Maximum, and Request matrices must have the same number of processes."
    if any(len(row) != m for matrix in (allocation, maximum, requests) for row in matrix):
        return "Every matrix row must have one value per resource type."
    if any(x < 0 for x in totals):
        return "Resource totals cannot be negative."
    for i in range(n):
        for j in range(m):
            if allocation[i][j] > maximum[i][j]:
                return f"Allocation for P{i}, R{j} cannot exceed its Maximum."
    for j in range(m):
        used = sum(allocation[i][j] for i in range(n))
        if used > totals[j]:
            return f"Allocated instances of R{j} ({used}) exceed its total ({totals[j]})."
    return None


def current_available():
    allocation = st.session_state["allocation"]
    totals = st.session_state["totals"]
    return [totals[j] - sum(row[j] for row in allocation) for j in range(len(totals))]


# --------------------------------- Sidebar --------------------------------
with st.sidebar:
    st.title("⚙️ Deadlock Lab")
    st.caption("Interactive Operating Systems Simulator")
    st.divider()
    st.subheader("Load a scenario")
    st.button("Load safe Banker example", use_container_width=True,
               on_click=initialize_state, args=(safe_scenario(),))
    st.button("Load deadlock example", use_container_width=True,
               on_click=initialize_state, args=(deadlock_scenario(),))
    st.button("Reset current scenario", use_container_width=True,
               on_click=initialize_state, args=(safe_scenario(),))
    st.divider()
    st.subheader("Configure dimensions")
    process_count_cfg = st.number_input(
        "Number of processes", min_value=1, max_value=10,
        value=int(st.session_state.get("process_count_cfg", len(st.session_state["allocation"]))),
        step=1, key="cfg_process_count",
    )
    resource_count_cfg = st.number_input(
        "Number of resource types", min_value=1, max_value=6,
        value=int(st.session_state.get("resource_count_cfg", len(st.session_state["totals"]))),
        step=1, key="cfg_resource_count",
    )

    def apply_dimensions():
        n = int(st.session_state["cfg_process_count"])
        m = int(st.session_state["cfg_resource_count"])
        st.session_state["totals"] = [max(1, st.session_state.get(f"total_{j}", 1)) for j in range(m)]
        st.session_state["allocation"] = [[0] * m for _ in range(n)]
        st.session_state["maximum"] = [[1] * m for _ in range(n)]
        st.session_state["requests"] = [[0] * m for _ in range(n)]
        st.session_state["events"] = [f"Created a blank scenario with {n} processes and {m} resource types."]
        st.session_state["last_result"] = "Blank scenario created. Edit the matrices to configure it."
        st.session_state["process_count_cfg"] = n
        st.session_state["resource_count_cfg"] = m
        st.session_state["cfg_process_count"] = n
        st.session_state["cfg_resource_count"] = m
        for key in ("allocation_editor", "maximum_editor", "requests_editor"):
            st.session_state.pop(key, None)
        for j in range(20):
            st.session_state.pop(f"total_{j}", None)
            st.session_state.pop(f"req_{j}", None)
            st.session_state.pop(f"release_{j}", None)

    st.button("Apply dimensions", type="primary", use_container_width=True, on_click=apply_dimensions)
    st.caption("Tip: use the built-in deadlock example to demonstrate detection and recovery.")

# ------------------------------ Main heading ------------------------------
st.title("Interactive Deadlock Simulation System")
st.caption("Explore resource allocation, safe states, deadlock detection, and recovery.")
st.markdown(
    """
    <style>
    .stApp {background: linear-gradient(145deg, #0b1220 0%, #101a2b 100%);}
    [data-testid="stMetric"] {background:rgba(27,43,64,.72);padding:16px;border:1px solid #2a3b53;border-radius:12px;}
    div[data-testid="stDataFrame"] {border:1px solid #293b53;border-radius:8px;overflow:hidden;}
    </style>
    """,
    unsafe_allow_html=True,
)

# Totals controls are intentionally visible and editable.
st.subheader("Resource inventory")
total_cols = st.columns(len(st.session_state["totals"]))
new_totals = []
for j, col in enumerate(total_cols):
    with col:
        new_totals.append(
            st.number_input(
                f"Total instances of R{j}", min_value=0, max_value=999,
                value=int(st.session_state["totals"][j]), step=1, key=f"total_{j}"
            )
        )
st.session_state["totals"] = [int(x) for x in new_totals]

allocation = st.session_state["allocation"]
maximum = st.session_state["maximum"]
requests = st.session_state["requests"]
totals = st.session_state["totals"]
available = current_available()
validation_error = validate_state(allocation, maximum, totals, requests)

# Tabs are all executed by Streamlit, so matrix editors remain in sync with state.
tab_overview, tab_matrices, tab_graph, tab_actions, tab_log = st.tabs(
    ["🏠 Overview", "📊 Matrices", "🔗 Resource Graph", "🧪 Requests & Recovery", "📝 Event Log"]
)

with tab_matrices:
    st.info("Edit the matrix cells directly. Need is calculated automatically as Maximum − Allocation.")
    left, right = st.columns(2)
    with left:
        st.markdown("#### Allocation matrix")
        allocation_df = st.data_editor(
            matrix_df(allocation), key="allocation_editor", use_container_width=True,
            num_rows="fixed", column_config={
                c: st.column_config.NumberColumn(c, min_value=0, step=1, format="%d")
                for c in matrix_df(allocation).columns
            },
        )
        st.markdown("#### Maximum matrix")
        maximum_df = st.data_editor(
            matrix_df(maximum), key="maximum_editor", use_container_width=True,
            num_rows="fixed", column_config={
                c: st.column_config.NumberColumn(c, min_value=0, step=1, format="%d")
                for c in matrix_df(maximum).columns
            },
        )
    with right:
        st.markdown("#### Outstanding Request matrix")
        request_df = st.data_editor(
            matrix_df(requests), key="requests_editor", use_container_width=True,
            num_rows="fixed", column_config={
                c: st.column_config.NumberColumn(c, min_value=0, step=1, format="%d")
                for c in matrix_df(requests).columns
            },
            
        )
        st.markdown("#### Need matrix")
        st.dataframe(
            matrix_df(calculate_need(allocation, maximum)),
            use_container_width=True,
        )
        st.markdown("#### Available vector")
        st.dataframe(pd.DataFrame([available], index=["Available"],
                                  columns=[f"R{j}" for j in range(len(available))]),
                     use_container_width=True)

    try:
        edited_allocation = read_editor_matrix(allocation_df)
        edited_maximum = read_editor_matrix(maximum_df)
        edited_requests = read_editor_matrix(request_df)
        error = validate_state(edited_allocation, edited_maximum, totals, edited_requests)
        if error:
            st.error(error)
        else:
            if edited_allocation != st.session_state["allocation"]:
                st.session_state["allocation"] = edited_allocation
            if edited_maximum != st.session_state["maximum"]:
                st.session_state["maximum"] = edited_maximum
            if edited_requests != st.session_state["requests"]:
                st.session_state["requests"] = edited_requests
    except (ValueError, TypeError) as exc:
        st.error(str(exc))

with tab_overview:
    allocation = st.session_state["allocation"]
    maximum = st.session_state["maximum"]
    totals = st.session_state["totals"]
    available = current_available()
    error = validate_state(allocation, maximum, totals, st.session_state["requests"])
    safe, sequence, unable = safety_check(allocation, maximum, available) if not error else (False, [], [])
    metric_cols = st.columns(4)
    metric_cols[0].metric("Processes", len(allocation))
    metric_cols[1].metric("Resource types", len(totals))
    metric_cols[2].metric("Available", " · ".join(map(str, available)))
    metric_cols[3].metric("Safety status", "SAFE" if safe else "UNSAFE")
    if error:
        st.error(error)
    elif safe:
        st.success("The current allocation is in a safe state.")
        st.write("Safe sequence: " + "  →  ".join(f"P{i}" for i in sequence))
    else:
        st.warning("The current allocation is unsafe. This does not automatically mean a deadlock exists.")
        if unable:
            st.write("Processes unable to finish in the safety check: " + ", ".join(f"P{i}" for i in unable))
    st.markdown("#### Current allocation snapshot")
    st.dataframe(matrix_df(allocation), use_container_width=True)
    st.markdown("#### Latest event")
    st.write(st.session_state["last_result"])
    check_col, detect_col = st.columns(2)
    with check_col:
        if st.button("▶ Run Banker's safety check", type="primary", use_container_width=True):
            if error:
                st.error(error)
            else:
                safe, sequence, unable = safety_check(allocation, maximum, available)
                if safe:
                    message = "SAFE state. Sequence: " + " → ".join(f"P{i}" for i in sequence)
                    st.success(message)
                else:
                    message = "UNSAFE state. Unable to finish: " + ", ".join(f"P{i}" for i in unable)
                    st.warning(message)
                log_event(message)
                st.rerun()
    with detect_col:
        if st.button("🔎 Detect deadlock", use_container_width=True):
            if error:
                st.error(error)
            else:
                deadlocked, completion = detect_deadlock(
                    allocation, available, st.session_state["requests"]
                )
                if deadlocked:
                    message = "Deadlock detected among: " + ", ".join(f"P{i}" for i in deadlocked)
                    st.error(message)
                else:
                    message = "No deadlock detected for the current outstanding requests."
                    st.success(message)
                log_event(message)
                st.rerun()

with tab_graph:
    allocation = st.session_state["allocation"]
    requests = st.session_state["requests"]
    totals = st.session_state["totals"]
    st.markdown("**Legend:** green arrows show allocated resources (R → P); dashed orange arrows show outstanding requests (P → R).")
    fig = draw_resource_allocation_graph(allocation, requests, totals)
    st.pyplot(fig, use_container_width=True)
    import matplotlib.pyplot as plt
    plt.close(fig)

with tab_actions:
    allocation = st.session_state["allocation"]
    maximum = st.session_state["maximum"]
    totals = st.session_state["totals"]
    available = current_available()
    process_options = [f"P{i}" for i in range(len(allocation))]
    selected_process = st.selectbox("Select process", range(len(allocation)),
                                    format_func=lambda i: f"P{i}", key="action_process")
    resource_cols = st.columns(len(totals))
    request_vector = []
    release_vector = []
    for j, col in enumerate(resource_cols):
        with col:
            request_vector.append(st.number_input(
                f"Request R{j}", min_value=0, max_value=999, value=0, step=1, key=f"req_{j}"
            ))
            release_vector.append(st.number_input(
                f"Release R{j}", min_value=0, max_value=999, value=0, step=1, key=f"release_{j}"
            ))

    req_col, rel_col = st.columns(2)
    with req_col:
        if st.button("Submit resource request", type="primary", use_container_width=True):
            error = validate_state(allocation, maximum, totals, st.session_state["requests"])
            if error:
                st.error(error)
            else:
                ok, message, new_allocation, new_available = try_request(
                    selected_process, [int(x) for x in request_vector],
                    allocation, maximum, available
                )
                if ok:
                    st.session_state["allocation"] = new_allocation
                    st.session_state["requests"][selected_process] = [0] * len(totals)
                    log_event(f"P{selected_process}: {message}")
                    st.success(message)
                else:
                    # Preserve a valid pending request for the deadlock detector if it fits Need.
                    need = calculate_need(allocation, maximum)
                    req = [int(x) for x in request_vector]
                    if (all(x >= 0 for x in req)
                        and all(req[j] <= need[selected_process][j] for j in range(len(totals)))
                        and any(req)):
                        st.session_state["requests"][selected_process] = req
                    log_event(f"P{selected_process}: {message}")
                    st.warning(message)
                st.rerun()
    with rel_col:
        if st.button("Release allocated resources", use_container_width=True):
            allocation_copy = copy.deepcopy(allocation)
            if any(int(release_vector[j]) > allocation_copy[selected_process][j] for j in range(len(totals))):
                st.error("Cannot release more of a resource than this process currently holds.")
            else:
                for j in range(len(totals)):
                    allocation_copy[selected_process][j] -= int(release_vector[j])
                st.session_state["allocation"] = allocation_copy
                log_event(f"P{selected_process} released {list(map(int, release_vector))}.")
                st.success("Resources released and Available recalculated.")
                st.rerun()

    st.divider()
    st.markdown("#### Deadlock recovery")
    st.warning("Terminating a process is a simulation action. It sets that process's Allocation, Maximum, and outstanding Request to zero, releasing its held resources.")
    if st.button("Terminate selected process and release its resources", type="secondary", use_container_width=True):
        allocation_copy = copy.deepcopy(st.session_state["allocation"])
        maximum_copy = copy.deepcopy(st.session_state["maximum"])
        requests_copy = copy.deepcopy(st.session_state["requests"])
        released = allocation_copy[selected_process][:]
        allocation_copy[selected_process] = [0] * len(totals)
        maximum_copy[selected_process] = [0] * len(totals)
        requests_copy[selected_process] = [0] * len(totals)
        st.session_state["allocation"] = allocation_copy
        st.session_state["maximum"] = maximum_copy
        st.session_state["requests"] = requests_copy
        log_event(f"Terminated P{selected_process}; released resources {released}.")
        st.success(f"P{selected_process} terminated. Its held resources are now available.")
        st.rerun()

with tab_log:
    st.markdown("The log records accepted requests, denied requests, checks, and recovery actions.")
    if st.button("Clear event log"):
        st.session_state["events"] = []
        st.rerun()
    if st.session_state["events"]:
        for idx, event in reversed(list(enumerate(st.session_state["events"], start=1))):
            st.markdown(f"**{idx:02d}.** {event}")
    else:
        st.caption("No events recorded yet.")
