import matplotlib.pyplot as plt
import networkx as nx


def draw_resource_allocation_graph(allocation, outstanding_requests, totals):
    """
    Draw a directed Resource Allocation Graph.
    Allocation edges: Resource -> Process.
    Request edges: Process -> Resource.
    Edge labels show the number of instances.
    """
    process_count = len(allocation)
    resource_count = len(totals)
    graph = nx.DiGraph()

    process_nodes = [f"P{i}" for i in range(process_count)]
    resource_nodes = [f"R{j}" for j in range(resource_count)]
    graph.add_nodes_from(process_nodes, kind="process")
    graph.add_nodes_from(resource_nodes, kind="resource")

    for i in range(process_count):
        for j in range(resource_count):
            allocated = allocation[i][j]
            requested = outstanding_requests[i][j]
            if allocated > 0:
                graph.add_edge(f"R{j}", f"P{i}", kind="allocation", amount=allocated)
            if requested > 0:
                graph.add_edge(f"P{i}", f"R{j}", kind="request", amount=requested)

    positions = {}
    for i, node in enumerate(process_nodes):
        positions[node] = (0, process_count - i)
    for j, node in enumerate(resource_nodes):
        positions[node] = (2.6, resource_count - j + (process_count - resource_count) / 2)

    fig, ax = plt.subplots(figsize=(10, max(4.2, process_count * 0.72)))
    fig.patch.set_facecolor("#0e1726")
    ax.set_facecolor("#0e1726")

    nx.draw_networkx_nodes(
        graph, positions, nodelist=process_nodes, node_shape="o",
        node_color="#254a78", edgecolors="#83b8ff", linewidths=1.8,
        node_size=1500, ax=ax
    )
    nx.draw_networkx_nodes(
        graph, positions, nodelist=resource_nodes, node_shape="s",
        node_color="#4a326f", edgecolors="#c6a6ff", linewidths=1.8,
        node_size=1700, ax=ax
    )

    allocation_edges = [
        (u, v) for u, v, d in graph.edges(data=True) if d["kind"] == "allocation"
    ]
    request_edges = [
        (u, v) for u, v, d in graph.edges(data=True) if d["kind"] == "request"
    ]

    nx.draw_networkx_edges(
        graph, positions, edgelist=allocation_edges, edge_color="#54d6a6",
        width=2.1, arrows=True, arrowsize=20, connectionstyle="arc3,rad=0.08",
        min_source_margin=20, min_target_margin=20, ax=ax
    )
    nx.draw_networkx_edges(
        graph, positions, edgelist=request_edges, edge_color="#ffb86b",
        width=2.1, style="dashed", arrows=True, arrowsize=20,
        connectionstyle="arc3,rad=-0.08", min_source_margin=20,
        min_target_margin=20, ax=ax
    )

    labels = {node: node for node in graph.nodes()}
    for j in range(resource_count):
        labels[f"R{j}"] = f"R{j}\nTotal: {totals[j]}"
    nx.draw_networkx_labels(graph, positions, labels=labels, font_color="#f5f7fb",
                            font_size=9, font_weight="bold", ax=ax)

    edge_labels = {(u, v): str(d["amount"]) for u, v, d in graph.edges(data=True)}
    if edge_labels:
        nx.draw_networkx_edge_labels(
            graph, positions, edge_labels=edge_labels, font_color="#ffffff",
            font_size=9, label_pos=0.5, rotate=False,
            bbox={"alpha": 0.75, "color": "#0e1726", "pad": 1.5}, ax=ax
        )

    ax.text(0.01, -0.07, "Green = allocated (Resource → Process)    Orange dashed = request (Process → Resource)",
            transform=ax.transAxes, color="#aebbd0", fontsize=9)
    ax.axis("off")
    fig.tight_layout()
    return fig
