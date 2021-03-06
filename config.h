/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int swallowfloating =
    1;                        /* 1 means swallow floating windows by default */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
static const char *fonts[] = {
    "Fira Code Medium:size=8",
    "Noto Color Emoji:size=8",
    "FuraCode Nerd Font:size=10",
};
static const char dmenufont[] = "Fira Code Retina:size=10";
static const char col_gray1[] = "#111111";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#ffffff";
static const char col_primary[] = "#2bafc6";
static const char col_secondary[] = "#11c654";

static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_primary, col_gray1, col_gray1},
    [SchemeSel] = {col_secondary, col_gray1, col_primary},
    [SchemeStatus] = {"#ffffff", "#26547c",
                      "#000000"}, // Statusbar right {text,background,not used
                                  // but cannot be empty}
    [SchemeTagsSel] = {"#edae49", col_gray1,
                       "#000000"}, // Tagbar left selected {text,background,not
                                   // used but cannot be empty}
    [SchemeTagsNorm] =
        {col_gray1, "#edae49",
         "#000000"}, // Tagbar left unselected {text,background,not used but
                     // cannot be empty}
    [SchemeInfoSel] =
        {"#000000", "#d1495b",
         "#000000"}, // infobar middle  selected {text,background,not used but
                     // cannot be empty}
    [SchemeInfoNorm] =
        {"#000000", "#d1495b",
         "#000000"}, // infobar middle  unselected {text,background,not used but
                     // cannot be empty}
};

/* tagging */
static const char *tags[] = {"1", "2", "???", "4", "???", "6", "7", "???", "???"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class, instance, title, tags mask, isfloating, isterminal, noswallow,
     * monitor
     */
    {"Yad", "yad", "Termbin Get", 0, 1, 0, 0, -1},
    {NULL, NULL, "Zoom Meeting", 1 << 8, 1, 0, 0, -1},
    {NULL, NULL, "Zoom", 1 << 8, 1, 0, 0, -1},
    {"Signal", NULL, NULL, 1 << 2, 0, 0, 0, -1},
    {"Xournalpp", NULL, NULL, 1 << 4, 0, 0, 0, -1},
    {"VSCodium", NULL, NULL, 1 << 7, 0, 0, 0, -1},
    {"Zotero", NULL, NULL, 1 << 4, 0, 0, 0, -1},
    {"Hamsket", NULL, NULL, 1 << 2, 0, 0, 0, -1},
    {"kitty", "kitty", NULL, 0, 0, 1, 0, -1},
    {"Anoise.py", NULL, NULL, 1 << 8, 1, 0, 1, -1},
    {"eww-cal", "eww-cal", NULL, 1 << 8, 1, 0, 1, -1},
    {"eww-mus", "eww-mus", NULL, 1 << 8, 1, 0, 1, -1},
    {"eww-mail", "eww-mail", NULL, 1 << 8, 1, 0, 1, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol   arrange function */
    {"T", tile}, /* first entry is default */
    {"X", NULL}, /* no layout function means floating behavior */
    {"M", monocle},
    {"_", clear},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}}, {                      \
    MODKEY | ControlMask | ShiftMask, KEY, toggletag, { .ui = 1 << TAG }       \
  }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }
#define RUNCMD(cmd)                                                            \
  {                                                                            \
    .v = (const char *[]) { cmd, NULL }                                        \
  }

/* commands */

/* Run applications */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *dmenucmd[] = {"dmenu-all", NULL};
static const char *signalcmd[] = {"signal-desktop", "--use-tray-icon", NULL};
static const char *sptcmd[] = {"kitty", "spt", NULL};
/*static const char *calccmd[] = {
    "kitty", "zsh", "-c",
    "CALCURSE_PAGER=/home/dhruva/.scripts/tools/calcurse-pager calcurse", NULL};
*/
static const char *copymenu[] = {"copyq", "menu", NULL};

#include "clear.c"
static Key keys[] = {
    /* modifie key function argument
     */
    {MODKEY, XK_Return, spawn, RUNCMD("kitty")},
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},

    {MODKEY, XK_F2, spawn, RUNCMD("firefox")},
    {MODKEY, XK_z, spawn, RUNCMD("zoom")},
    {MODKEY, XK_w, spawn, RUNCMD("hamsket")},
    {MODKEY | ShiftMask, XK_z, spawn, RUNCMD("zotero")},
    {MODKEY | ShiftMask, XK_d, spawn, RUNCMD("hamsket")},
    {MODKEY, XF86XK_AudioPlay, spawn, {.v = sptcmd}},
    {0 | ShiftMask, XF86XK_AudioPlay, spawn, SHCMD("mpv --loop ~/Music/.noise/*")},
    {0 | ControlMask, XF86XK_AudioPlay, spawn, SHCMD("pkill mpv")},
    {MODKEY, XK_s, spawn, {.v = signalcmd}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY, XK_o, winview, {0}},
    {MODKEY | ShiftMask, XK_t, spawn, RUNCMD("todo")},

    {MODKEY | ShiftMask, XK_p, spawn, RUNCMD("pavucontrol")},
    {MODKEY, XK_e, spawn, RUNCMD("nautilus")},
    {MODKEY, XK_v, spawn, {.v = copymenu}},

    {MODKEY | ControlMask, XK_s, spawn,
     SHCMD("maim -s > $( find ~/idrive -type d | dmenu )/$(date "
           "+%m-%d-%H-%M).png")},
    {MODKEY | ShiftMask, XK_s, spawn,
     SHCMD("maim -s | copyq write image/png - && copyq select 0")},
    {MODKEY, XK_f, spawn,
     SHCMD("feh --randomize gdrive/wallpaper --bg-fill --no-fehbg && "
           "statusbar-update")},

    {MODKEY | ShiftMask, XK_Return, zoom, {0}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},

    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_x, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_minus, togglebar, {0}},
    {MODKEY, XK_minus, setlayout, {.v = &layouts[3]}},

    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_l, spawn, SHCMD("loginctl lock-session")},
    {MODKEY | ShiftMask, XK_q, killclient, {0}},

    {MODKEY, XK_Menu, spawn,
     SHCMD("killall trayer; trayer --expand true --widthtype request --tint "
           "255 --edge bottom --align right --height 40 --iconspacing 4&")},
    {MODKEY | ShiftMask, XK_Menu, spawn, SHCMD("killall trayer")},

    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("light -U 10")},
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("light -A 10")},
    {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer -i 5 && audio-notify")},
    {0, XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer -d 5 && audio-notify")},
    {MODKEY, XF86XK_AudioMute, spawn,
     SHCMD("dunstctl set-paused toggle && statusbar-update")},
    {0, XF86XK_AudioMute, spawn, SHCMD("pamixer -t && audio-notify")},
    {0, XF86XK_AudioNext, spawn, SHCMD("playerctl -a next")},
    {0, XF86XK_AudioPlay, spawn, SHCMD("playerctl -a play-pause")},
    {0, XF86XK_AudioPrev, spawn, SHCMD("playerctl -a previous")},

    {MODKEY, XK_Delete, spawn, RUNCMD("networkmanager_dmenu")},
    {MODKEY, XK_Insert, spawn, RUNCMD("bluetoothctl-dmenu")},

    TAGKEYS(XK_1, 0),
    TAGKEYS(XK_2, 1),
    TAGKEYS(XK_3, 2),
    TAGKEYS(XK_4, 3),
    TAGKEYS(XK_5, 4),
    TAGKEYS(XK_6, 5),
    TAGKEYS(XK_7, 6),
    TAGKEYS(XK_8, 7),
    TAGKEYS(XK_9, 8),

    {MODKEY | ShiftMask, XK_x, spawn, SHCMD("systemctl suspend")},
    {MODKEY | ControlMask, XK_x, spawn, SHCMD("systemctl hybrid-sleep")},

    {MODKEY | ShiftMask, XK_c, quit, {.i = EXIT_SUCCESS}},
    {MODKEY | ControlMask, XK_c, quit, {.i = EXIT_FAILURE}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask          button          function
       argument */
    {ClkLtSymbol, 0, Button1, setlayout, {.v = &layouts[3]}},
    {ClkLtSymbol, 0, Button1, togglebar, {0}},
    {ClkStatusText, 0, Button1, spawn, RUNCMD("pavucontrol")},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button3, togglefloating, {0}},
    {ClkClientWin, MODKEY | ShiftMask, Button1, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
    {ClkRootWin, 0, Button1, spawn, RUNCMD("kitty")},
    {ClkRootWin, 0, Button3, spawn, RUNCMD("dmenu-all")},
    {ClkRootWin, MODKEY, Button1, spawn, RUNCMD("system-monitoring-center")},
    {ClkWinTitle, 0, Button1, setlayout, {.v = &layouts[3]}},
    {ClkWinTitle, 0, Button1, togglebar, {0}},
    {ClkWinTitle, 0, Button3, spawn, RUNCMD("dmenu-all")},
};
