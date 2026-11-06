import ctypes
import os
import tkinter as tk
from tkinter import simpledialog, messagebox
import tkinter.scrolledtext as st

# Load C Shared Library
lib_path = os.path.join(os.path.dirname(__file__), "libdeadlock.so")
try:
    lib = ctypes.CDLL(lib_path)
except OSError:
    print("Error: Compile the C backend using 'make' first.")
    exit(1)

# C-types configurations
IntArrayMax = ctypes.c_int * 20
lib.run_bankers.argtypes = [ctypes.POINTER(ctypes.c_int)]
lib.request_resources.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_int)]
lib.detect_deadlock.argtypes = [ctypes.POINTER(ctypes.c_int)]

class DeadlockGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Deadlock Simulation System - KBSR")
        self.root.geometry("900x650")
        self.root.configure(bg="#1e1e2e")
        
        self.n_p = 0
        self.n_r = 0
        
        self.setup_ui()

    def setup_ui(self):
        # Left Panel (Controls)
        ctrl_frame = tk.Frame(self.root, bg="#1e1e2e")
        ctrl_frame.pack(side=tk.LEFT, fill=tk.Y, padx=20, pady=20)
        
        tk.Label(ctrl_frame, text="DEADLOCK SIMULATOR", font=("Arial", 16, "bold"), bg="#1e1e2e", fg="#94e2d5").pack(pady=10)

        buttons = [
            ("1. Init System (Procs & Res)", self.init_system, "#89b4fa"),
            ("2. Set Available Vector", self.set_available, "#89b4fa"),
            ("3. Add Process (Max & Alloc)", self.add_process, "#89b4fa"),
            ("4. Display Need & State", self.display_state, "#cba6f7"),
            ("5. Resource Allocation Graph", self.display_rag, "#cba6f7"),
            ("6. Request Resources", self.req_resources, "#f9e2af"),
            ("7. Run Banker's Algorithm", self.run_bankers, "#a6e3a1"),
            ("8. Detect Deadlock", self.detect_dl, "#f38ba8"),
            ("9. Recover from Deadlock", self.recover_dl, "#eba0ac"),
            ("10. Exit", self.root.quit, "#6c7086")
        ]

        for text, cmd, color in buttons:
            tk.Button(ctrl_frame, text=text, command=cmd, bg=color, fg="#11111b", font=("Arial", 11, "bold"), 
                      width=25, pady=5, borderwidth=0).pack(pady=5)

        # Right Panel (Console Output)
        self.console = st.ScrolledText(self.root, bg="#11111b", fg="#cdd6f4", font=("Consolas", 11), borderwidth=0)
        self.console.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=20, pady=20)
        self.log("System Ready. Initialize processes and resources to begin.")

    def log(self, message):
        self.console.insert(tk.END, message + "\n")
        self.console.see(tk.END)

    def get_array_input(self, prompt, length):
        val = simpledialog.askstring("Input", f"{prompt} (comma separated, {length} values):")
        if not val: return None
        arr = [int(x.strip()) for x in val.split(',')]
        if len(arr) != length:
            messagebox.showerror("Error", f"Expected {length} values.")
            return None
        c_arr = IntArrayMax(*arr)
        return c_arr

    def init_system(self):
        self.n_p = simpledialog.askinteger("Input", "Number of Processes (max 20):")
        self.n_r = simpledialog.askinteger("Input", "Number of Resources (max 20):")
        if self.n_p and self.n_r:
            lib.init_system(self.n_p, self.n_r)
            self.console.delete(1.0, tk.END)
            self.log(f"System initialized with {self.n_p} processes and {self.n_r} resources.")

    def set_available(self):
        arr = self.get_array_input("Available Resources Vector", self.n_r)
        if arr:
            lib.set_available(arr)
            self.log("Available resources updated.")

    def add_process(self):
        pid = simpledialog.askinteger("Input", "Process ID (0 to N-1):")
        if pid is not None and 0 <= pid < self.n_p:
            max_arr = self.get_array_input(f"Max Matrix for P{pid}", self.n_r)
            if max_arr:
                alloc_arr = self.get_array_input(f"Allocation Matrix for P{pid}", self.n_r)
                if alloc_arr:
                    lib.set_process_data(pid, max_arr, alloc_arr)
                    self.log(f"Matrices configured for Process P{pid}.")

    def display_state(self):
        self.log("\n--- SYSTEM STATE ---")
        self.log("Avail: " + " ".join(str(lib.get_avail(j)) for j in range(self.n_r)))
        self.log("PID | Allocation | Max      | Need")
        for i in range(self.n_p):
            if lib.get_finished(i):
                self.log(f"P{i}  | ABORTED / FINISHED")
                continue
            alloc = " ".join(str(lib.get_alloc(i, j)) for j in range(self.n_r))
            mx = " ".join(str(lib.get_max(i, j)) for j in range(self.n_r))
            nd = " ".join(str(lib.get_need(i, j)) for j in range(self.n_r))
            self.log(f"P{i}  | {alloc:<10} | {mx:<8} | {nd}")

    def display_rag(self):
        self.log("\n--- RESOURCE ALLOCATION GRAPH (RAG) Edges ---")
        edges_exist = False
        for p in range(self.n_p):
            if lib.get_finished(p): continue
            for r in range(self.n_r):
                alloc = lib.get_alloc(p, r)
                need = lib.get_need(p, r)
                if alloc > 0:
                    self.log(f"[Assignment Edge] Resource R{r} --({alloc})--> Process P{p}")
                    edges_exist = True
                if need > 0:
                    self.log(f"[Request Edge]    Process P{p} --(needs {need})--> Resource R{r}")
                    edges_exist = True
        if not edges_exist:
            self.log("Graph is currently empty.")

    def req_resources(self):
        pid = simpledialog.askinteger("Input", "Requesting Process ID:")
        if pid is not None:
            req = self.get_array_input(f"Request Vector for P{pid}", self.n_r)
            if req:
                res = lib.request_resources(pid, req)
                if res == 1: self.log(f"SUCCESS: Resources allocated to P{pid}. State is Safe.")
                elif res == 0: self.log(f"DENIED: Allocating to P{pid} results in Unsafe state. Rolled back.")
                elif res == -1: self.log(f"ERROR: P{pid} requested more than its declared need.")
                elif res == -2: self.log(f"WAIT: Resources not currently available for P{pid}.")

    def run_bankers(self):
        seq = IntArrayMax()
        is_safe = lib.run_bankers(seq)
        if is_safe:
            safe_list = [f"P{seq[i]}" for i in range(self.n_p) if not lib.get_finished(seq[i])]
            self.log(f"\n[SAFE STATE] Safe Sequence: {' -> '.join(safe_list)}")
        else:
            self.log("\n[UNSAFE STATE] The system cannot guarantee all processes will finish.")

    def detect_dl(self):
        d_procs = IntArrayMax()
        count = lib.detect_deadlock(d_procs)
        if count == 0:
            self.log("\n[OK] No deadlock detected.")
        else:
            p_list = [f"P{d_procs[i]}" for i in range(count)]
            self.log(f"\n[DEADLOCK DETECTED] Deadlocked Processes: {', '.join(p_list)}")

    def recover_dl(self):
        victim = lib.recover_deadlock()
        if victim == -1:
            self.log("\nNo deadlock to recover from.")
        else:
            self.log(f"\n[RECOVERY] Aborted victim Process P{victim}. Resources preempted and added to available.")
            self.display_state()

if __name__ == "__main__":
    root = tk.Tk()
    app = DeadlockGUI(root)
    root.mainloop()
