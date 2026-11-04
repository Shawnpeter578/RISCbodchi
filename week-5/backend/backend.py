import ctypes
from pathlib import Path

MAX_P = 10
MAX_R = 10
DLL_PATH = Path(__file__).resolve().parents[1] / "backend" / "deadlock.dll"

class CBackend:
    def __init__(self):
        if not DLL_PATH.exists():
            raise FileNotFoundError(
                f"Could not find {DLL_PATH}. Build it first using backend\\build.bat."
            )
        self.dll = ctypes.CDLL(str(DLL_PATH))
        self._configure()

    def _configure(self):
        I = ctypes.c_int
        C = ctypes.c_char
        self.dll.ds_initialize.argtypes = [I, I, ctypes.POINTER(I), ctypes.POINTER(I),
            ctypes.POINTER(I), ctypes.POINTER(C), I]
        self.dll.ds_initialize.restype = I
        self.dll.ds_get_dimensions.argtypes = [ctypes.POINTER(I), ctypes.POINTER(I)]
        self.dll.ds_get_dimensions.restype = I
        self.dll.ds_get_state.argtypes = [ctypes.POINTER(I), ctypes.POINTER(I),
            ctypes.POINTER(I), ctypes.POINTER(I), ctypes.POINTER(I),
            ctypes.POINTER(I), ctypes.POINTER(I)]
        self.dll.ds_get_state.restype = I
        self.dll.ds_banker.argtypes = [ctypes.POINTER(C), I, ctypes.POINTER(I),
            ctypes.POINTER(I), ctypes.POINTER(I)]
        self.dll.ds_banker.restype = I
        self.dll.ds_request.argtypes = [I, ctypes.POINTER(I), ctypes.POINTER(C), I]
        self.dll.ds_request.restype = I
        self.dll.ds_detect.argtypes = [ctypes.POINTER(I), ctypes.POINTER(I)]
        self.dll.ds_detect.restype = I
        self.dll.ds_recover.argtypes = [ctypes.POINTER(I)]
        self.dll.ds_recover.restype = I
        self.dll.ds_get_rag.argtypes = [ctypes.POINTER(C), I]
        self.dll.ds_get_rag.restype = I
        self.dll.ds_reset.argtypes = []

    @staticmethod
    def _arr(values):
        return (ctypes.c_int * len(values))(*values)

    @staticmethod
    def _message(buf):
        return bytes(buf).split(b"\0", 1)[0].decode("utf-8", errors="replace")

    def initialize(self, total, allocation, maximum):
        p, r = len(allocation), len(total)
        alloc_flat = [x for row in allocation for x in row]
        max_flat = [x for row in maximum for x in row]
        msg = ctypes.create_string_buffer(1024)
        ok = self.dll.ds_initialize(p, r, self._arr(total),
            self._arr(alloc_flat), self._arr(max_flat), msg, len(msg))
        if not ok:
            raise ValueError(self._message(msg))
        return self._message(msg)

    def state(self):
        p, r = ctypes.c_int(), ctypes.c_int()
        if not self.dll.ds_get_dimensions(ctypes.byref(p), ctypes.byref(r)):
            raise RuntimeError("Initialize the system first.")
        total = (ctypes.c_int * MAX_R)()
        available = (ctypes.c_int * MAX_R)()
        size = MAX_P * MAX_R
        alloc = (ctypes.c_int * size)()
        maximum = (ctypes.c_int * size)()
        need = (ctypes.c_int * size)()
        active = (ctypes.c_int * MAX_P)()
        requests = (ctypes.c_int * size)()
        self.dll.ds_get_state(total, available, alloc, maximum, need, active, requests)
        def matrix(arr):
            return [[arr[i*r+j] for j in range(r)] for i in range(p.value)]
        return {
            "p": p.value, "r": r.value,
            "total": list(total[:r.value]),
            "available": list(available[:r.value]),
            "allocation": matrix(alloc), "maximum": matrix(maximum),
            "need": matrix(need), "active": list(active[:p.value]),
            "requests": matrix(requests),
        }

    def banker(self):
        msg = ctypes.create_string_buffer(2048)
        safe, length = ctypes.c_int(), ctypes.c_int()
        seq = (ctypes.c_int * MAX_P)()
        ok = self.dll.ds_banker(msg, len(msg), ctypes.byref(safe), seq, ctypes.byref(length))
        if not ok:
            raise RuntimeError("Initialize the system first.")
        return bool(safe.value), list(seq[:length.value]), self._message(msg)

    def request(self, process, values):
        msg = ctypes.create_string_buffer(1024)
        code = self.dll.ds_request(process, self._arr(values), msg, len(msg))
        return code, self._message(msg)

    def detect(self):
        flags = (ctypes.c_int * MAX_P)()
        count = ctypes.c_int()
        if not self.dll.ds_detect(flags, ctypes.byref(count)):
            raise RuntimeError("Initialize the system first.")
        return [i for i, flag in enumerate(flags[:self.state()["p"]]) if flag], count.value

    def recover(self):
        recovered = ctypes.c_int()
        if not self.dll.ds_recover(ctypes.byref(recovered)):
            raise RuntimeError("Initialize the system first.")
        return recovered.value

    def rag(self):
        buf = ctypes.create_string_buffer(8192)
        if not self.dll.ds_get_rag(buf, len(buf)):
            raise RuntimeError("Initialize the system first.")
        return self._message(buf)
