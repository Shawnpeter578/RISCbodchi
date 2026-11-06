#!/usr/bin/env python3
import json
import os
import socket
import sys
import tkinter as tk
from tkinter import messagebox, ttk

SOCK_DEFAULT = os.environ.get("PAGE_SIM_SOCKET", "./page_sim.sock")
PROTO_EXPECTED = 1
TIMEOUT_S = 5.0

class Backend:
    def __init__(self, path: str):
        self.path = path
        self.sock = None

    def connect(self):
        s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        s.settimeout(TIMEOUT_S)
        s.connect(self.path)
        self.sock = s
        pong = self.request({"cmd": "ping"})
        v = pong.get("v", -1)
        if v != PROTO_EXPECTED:
            raise RuntimeError(f"protocol mismatch: backend speaks v{v}")

    def request(self, obj: dict) -> dict:
        line = json.dumps(obj, separators=(",", ":")) + "\n"
        self.sock.sendall(line.encode("utf-8"))
        buf = bytearray()
        while True:
            chunk = self.sock.recv(65536)
            buf.extend(chunk)
            nl = buf.find(b"\n")
            if nl >= 0:
                return json.loads(bytes(buf[:nl]).decode("utf-8"))

    def close(self):
        if self.sock:
            try: self.sock.sendall(b'{"cmd":"quit"}\n')
            except: pass
            self.sock.close()
            self.sock = None

class App:
    def __init__(self, root: tk.Tk, backend: Backend):
        self.root = root
        self.backend = backend
        root.title("Page Replacement Simulator")
        root.geometry("1000x650")

        top = ttk.Frame(root, padding=(10, 10, 10, 4))
        top.pack(fill="x")
        ttk.Label(top, text="refs ; frames ; algo(1-4)").pack(anchor="w")
        row = ttk.Frame(top)
        row.pack(fill="x", pady=(2, 0))

        self.entry = ttk.Entry(row)
        self.entry.pack(side="left", fill="x", expand=True)
        self.entry.insert(0, "1 2 3 4 1 2 5 ; 3 ; 4")
        ttk.Button(row, text="Run", command=self.run).pack(side="left")

        self.status = tk.StringVar(value="ready")
        ttk.Label(root, textvariable=self.status, padding=(10, 0, 10, 6)).pack(fill="x")

        sel = ttk.Frame(root, padding=(10, 0, 10, 6))
        sel.pack(fill="x")
        ttk.Label(sel, text="Show trace for:").pack(side="left")
        self.algo_pick = ttk.Combobox(sel, state="readonly", width=14)
        self.algo_pick.pack(side="left", padx=(6, 0))
        self.algo_pick.bind("<<ComboboxSelected>>", lambda _e: self.render_trace())

        self.tv = ttk.Treeview(root, show="headings")
        self.tv.pack(fill="both", expand=True, padx=10, pady=(0, 10))

        ttk.Label(root, text="Comparison").pack(anchor="w", padx=10)
        self.tv_cmp = ttk.Treeview(root, show="headings", height=4, columns=("algo", "hits", "faults", "rate", "best"))
        for col, w in (("algo", 160), ("hits", 80), ("faults", 80), ("rate", 100), ("best", 80)):
            self.tv_cmp.heading(col, text=col)
            self.tv_cmp.column(col, width=w, anchor="center")
        self.tv_cmp.pack(fill="x", padx=10, pady=(0, 10))
        self.last_response = None

    def run(self):
        try:
            parts = [p.strip() for p in self.entry.get().split(";")]
            req = {"cmd": "run", "refs": [int(x) for x in parts[0].split()], "frames": int(parts[1]), "algo": int(parts[2])}
            resp = self.backend.request(req)
        except Exception as e:
            messagebox.showerror("Error", str(e))
            return
        
        self.last_response = resp
        self.algo_pick["values"] = [r["name"] for r in resp.get("results", [])]
        if self.algo_pick["values"]: self.algo_pick.set(self.algo_pick["values"][0])
        self.render_trace()
        self.render_comparison(resp)

    def render_trace(self):
        self.tv.delete(*self.tv.get_children())
        if not self.last_response: return
        r = next((x for x in self.last_response["results"] if x["name"] == self.algo_pick.get()), None)
        if not r: return

        cols = ["page"] + [f"F{i}" for i in range(r["frames"])] + ["status", "victim"]
        self.tv["columns"] = cols
        for c in cols:
            self.tv.heading(c, text=c)
            self.tv.column(c, width=70 if c in ("page", "status", "victim") else 50, anchor="center")

        for step in r["steps"]:
            frames = ["." if v is None else v for v in step["frames"]]
            victim = "-" if step["victim"] < 0 else step["victim"]
            self.tv.insert("", "end", values=[step["page"]] + frames + [step["status"], victim])

    def render_comparison(self, resp):
        self.tv_cmp.delete(*self.tv_cmp.get_children())
        best = resp.get("best", -1)
        for i, r in enumerate(resp["results"]):
            rate = f"{100.0 * r['hits'] / (r['total_refs'] or 1):.1f}%"
            self.tv_cmp.insert("", "end", values=(r["name"], r["hits"], r["faults"], rate, "BEST" if i == best else ""))

def main():
    backend = Backend(sys.argv[1] if len(sys.argv) > 1 else SOCK_DEFAULT)
    backend.connect()
    root = tk.Tk()
    app = App(root, backend)
    root.protocol("WM_DELETE_WINDOW", lambda: (backend.close(), root.destroy()))
    root.mainloop()

if __name__ == "__main__":
    main()
