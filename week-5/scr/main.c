#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulator.h"
#include "parser.h"

#define ID_INPUT       101
#define ID_FRAMES      102
#define ID_ALGORITHM   103
#define ID_RUN         104
#define ID_RESULTS     105
#define ID_SUMMARY     106
#define ID_CLEAR       107

static HWND hInput, hFrames, hAlgorithm, hResults, hSummary;
static HFONT hFont;
static const char CLASS_NAME[] = "PageReplacementSimulatorWindow";

static void set_font(HWND control) {
    SendMessage(control, WM_SETFONT, (WPARAM)hFont, TRUE);
}

static void add_column(HWND list, const char *title, int width, int index) {
    LVCOLUMNA col;
    memset(&col, 0, sizeof(col));
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    col.pszText = (LPSTR)title;
    col.cx = width;
    col.iSubItem = index;
    ListView_InsertColumn(list, index, &col);
}

static void setup_columns(int frame_count) {
    int i;
    char title[32];
    while (ListView_DeleteColumn(hResults, 0)) {}
    add_column(hResults, "Algorithm", 90, 0);
    add_column(hResults, "Step", 55, 1);
    add_column(hResults, "Page", 55, 2);
    for (i = 0; i < frame_count; ++i) {
        snprintf(title, sizeof(title), "Frame %d", i + 1);
        add_column(hResults, title, 65, i + 3);
    }
    add_column(hResults, "Status", 75, frame_count + 3);
    add_column(hResults, "Replaced", 75, frame_count + 4);
}

static void add_result_rows(const SimulationResult *r) {
    int i, j, col;
    char text[64];
    LVITEMA item;

    for (i = 0; i < r->reference_count; ++i) {
        const SimulationStep *step = &r->steps[i];
        memset(&item, 0, sizeof(item));
        item.mask = LVIF_TEXT;
        item.iItem = ListView_GetItemCount(hResults);
        snprintf(text, sizeof(text), "%s", algorithm_name(r->algorithm));
        item.pszText = text;
        ListView_InsertItem(hResults, &item);

        col = 1;
        snprintf(text, sizeof(text), "%d", i + 1);
        ListView_SetItemText(hResults, item.iItem, col++, text);
        snprintf(text, sizeof(text), "%d", step->page);
        ListView_SetItemText(hResults, item.iItem, col++, text);

        for (j = 0; j < r->frame_count; ++j) {
            if (step->frames[j] < 0) strcpy(text, "-");
            else snprintf(text, sizeof(text), "%d", step->frames[j]);
            ListView_SetItemText(hResults, item.iItem, col++, text);
        }
        ListView_SetItemText(hResults, item.iItem, col++,
                             step->hit ? "HIT" : "FAULT");
        if (step->replaced_page < 0) strcpy(text, "-");
        else snprintf(text, sizeof(text), "%d", step->replaced_page);
        ListView_SetItemText(hResults, item.iItem, col, text);
    }
}

static void append_summary(char *buffer, size_t size,
                           const SimulationResult *r) {
    char line[256];
    double hit_ratio = 100.0 * r->hits / r->reference_count;
    double fault_ratio = 100.0 * r->faults / r->reference_count;
    snprintf(line, sizeof(line),
             "%-9s | Hits: %3d | Faults: %3d | Hit ratio: %6.2f%% | Fault ratio: %6.2f%%\r\n",
             algorithm_name(r->algorithm), r->hits, r->faults,
             hit_ratio, fault_ratio);
    strncat(buffer, line, size - strlen(buffer) - 1);
}

static void run_clicked(void) {
    char input[4096], frame_text[64], error[256];
    int refs[MAX_REFERENCES], count = 0, frames, choice, i;
    SimulationResult result;
    char summary[2048] = "";
    char *end;
    long parsed_frames;
    int algorithms[3] = { ALG_FIFO, ALG_LRU, ALG_OPTIMAL };
    int first, last;

    GetWindowTextA(hInput, input, sizeof(input));
    GetWindowTextA(hFrames, frame_text, sizeof(frame_text));

    if (!parse_references(input, refs, &count, error, sizeof(error))) {
        MessageBoxA(NULL, error, "Input error", MB_ICONWARNING);
        return;
    }

    parsed_frames = strtol(frame_text, &end, 10);
    if (end == frame_text || *end != '\0' || parsed_frames < 1 ||
        parsed_frames > MAX_FRAMES) {
        MessageBoxA(NULL, "Frame count must be an integer from 1 to 32.",
                    "Input error", MB_ICONWARNING);
        return;
    }
    frames = (int)parsed_frames;
    choice = (int)SendMessage(hAlgorithm, CB_GETCURSEL, 0, 0);

    if (choice == CB_ERR) {
        MessageBoxA(NULL, "Select an algorithm.", "Input error", MB_ICONWARNING);
        return;
    }

    first = (choice == 3) ? 0 : choice;
    last = (choice == 3) ? 2 : choice;

    ListView_DeleteAllItems(hResults);
    setup_columns(frames);

    for (i = first; i <= last; ++i) {
        if (!run_simulation((Algorithm)algorithms[i], refs, count, frames, &result)) {
            MessageBoxA(NULL, "Simulation failed. Check the input values.",
                        "Error", MB_ICONERROR);
            return;
        }
        add_result_rows(&result);
        append_summary(summary, sizeof(summary), &result);
    }

    SetWindowTextA(hSummary, summary);
}

static void clear_clicked(void) {
    SetWindowTextA(hInput, "");
    SetWindowTextA(hFrames, "3");
    SendMessage(hAlgorithm, CB_SETCURSEL, 3, 0);
    ListView_DeleteAllItems(hResults);
    SetWindowTextA(hSummary, "Results will appear here.");
}

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HWND label;
            INITCOMMONCONTROLSEX icc;
            icc.dwSize = sizeof(icc);
            icc.dwICC = ICC_LISTVIEW_CLASSES;
            InitCommonControlsEx(&icc);

            hFont = CreateFontA(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

            label = CreateWindowA("STATIC", "PAGE REPLACEMENT ALGORITHM SIMULATOR",
                WS_CHILD | WS_VISIBLE, 20, 15, 650, 32, hwnd, NULL, NULL, NULL);
            set_font(label);

            label = CreateWindowA("STATIC", "Page-reference string (spaces or commas):",
                WS_CHILD | WS_VISIBLE, 20, 60, 300, 24, hwnd, NULL, NULL, NULL);
            set_font(label);
            hInput = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT",
                "7 0 1 2 0 3 0 4 2 3",
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                20, 86, 500, 30, hwnd, (HMENU)ID_INPUT, NULL, NULL);
            set_font(hInput);

            label = CreateWindowA("STATIC", "Frames:",
                WS_CHILD | WS_VISIBLE, 540, 60, 70, 24, hwnd, NULL, NULL, NULL);
            set_font(label);
            hFrames = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "3",
                WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER,
                540, 86, 70, 30, hwnd, (HMENU)ID_FRAMES, NULL, NULL);
            set_font(hFrames);

            label = CreateWindowA("STATIC", "Algorithm:",
                WS_CHILD | WS_VISIBLE, 630, 60, 90, 24, hwnd, NULL, NULL, NULL);
            set_font(label);
            hAlgorithm = CreateWindowA("COMBOBOX", "",
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                630, 86, 170, 150, hwnd, (HMENU)ID_ALGORITHM, NULL, NULL);
            SendMessageA(hAlgorithm, CB_ADDSTRING, 0, (LPARAM)"FIFO");
            SendMessageA(hAlgorithm, CB_ADDSTRING, 0, (LPARAM)"LRU");
            SendMessageA(hAlgorithm, CB_ADDSTRING, 0, (LPARAM)"Optimal");
            SendMessageA(hAlgorithm, CB_ADDSTRING, 0, (LPARAM)"Compare all");
            SendMessage(hAlgorithm, CB_SETCURSEL, 3, 0);
            set_font(hAlgorithm);

            {
                HWND button = CreateWindowA("BUTTON", "Run Simulation",
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    820, 84, 145, 34, hwnd, (HMENU)ID_RUN, NULL, NULL);
                set_font(button);
                button = CreateWindowA("BUTTON", "Clear",
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    975, 84, 80, 34, hwnd, (HMENU)ID_CLEAR, NULL, NULL);
                set_font(button);
            }

            label = CreateWindowA("STATIC", "Step-by-step memory state",
                WS_CHILD | WS_VISIBLE, 20, 135, 350, 24, hwnd, NULL, NULL, NULL);
            set_font(label);

            hResults = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEWA, "",
                WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                20, 162, 1035, 390, hwnd, (HMENU)ID_RESULTS, NULL, NULL);
            ListView_SetExtendedListViewStyle(hResults,
                LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
            set_font(hResults);
            setup_columns(3);

            label = CreateWindowA("STATIC", "Performance summary",
                WS_CHILD | WS_VISIBLE, 20, 565, 250, 24, hwnd, NULL, NULL, NULL);
            set_font(label);
            hSummary = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT",
                "Results will appear here.",
                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
                20, 592, 1035, 105, hwnd, (HMENU)ID_SUMMARY, NULL, NULL);
            set_font(hSummary);
            return 0;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_RUN:
                    run_clicked();
                    return 0;
                case ID_CLEAR:
                    clear_clicked();
                    return 0;
            }
            break;

        case WM_SIZE:
            if (hResults) {
                int width = LOWORD(lParam) - 40;
                int height = HIWORD(lParam);
                if (width < 400) width = 400;
                MoveWindow(hResults, 20, 162, width, height - 270, TRUE);
                MoveWindow(hSummary, 20, height - 125, width, 105, TRUE);
            }
            return 0;

        case WM_DESTROY:
            if (hFont) DeleteObject(hFont);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command_line, int show) {
    WNDCLASSA wc;
    HWND hwnd;
    MSG msg;
    (void)previous;
    (void)command_line;

    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "Could not register the application window.",
                    "Startup error", MB_ICONERROR);
        return 1;
    }

    hwnd = CreateWindowExA(0, CLASS_NAME,
        "Page Replacement Simulator | Operating Systems",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 780,
        NULL, NULL, instance, NULL);

    if (!hwnd) return 1;
    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
