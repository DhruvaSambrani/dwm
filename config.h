/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>


/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Fira Code:size=10", "Noto Color Emoji:size=10" };
static const char dmenufont[]       = "Fira Code Retina:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_primary[]     = "#cc0047";
static const char col_secondary[]   = "#5907b7";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_secondary },
	[SchemeSel]  = { col_gray4, col_gray2, col_primary   },
};

/* tagging */
static const char *tags[] = { "1", "2", "Discord", "Signal", "5", "6", "7", "8", "Zoom" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance    title       tags mask       isfloating  monitor */
	{ "Yad",        "yad",      "Help",     0,              1,          -1 },
	{ "zoom",       NULL,       NULL,       1<<8,           1,          -1 },
	{ "discord",    NULL,       NULL,       1<<2,           0,          -1 },
	{ "Signal",     NULL,       NULL,       1<<3,           0,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol   arrange function */
	{ "T",      tile    },    /* first entry is default */
	{ "X",      NULL    },    /* no layout function means floating behavior */
	{ "M",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define RUNCMD(cmd) { .v = (const char*[]){ cmd, NULL } }

/* commands */

/* Run applications */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *dmenucmd[]     = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_primary, "-sf", col_gray4, NULL };
static const char *gtk_dmenucmd[] = { "gtk_dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_primary, "-sf", col_gray4, NULL };
static const char *signalcmd[]    = { "signal-desktop", "--start-in-tray", "--use-tray-icon", NULL };
static const char *sptcmd[]       = { "kitty", "spt", NULL };
static const char *calccmd[]      = { "kitty", "zsh", "-c", "CALCURSE_PAGER=/home/dhruva/.scripts/tools/calcurse-pager calcurse", NULL };
static const char *copymenu[]     = { "copyq", "menu", NULL};

static Key keys[] = {
	/* modifier             key                         function        argument */
	{ MODKEY,               XK_Return,                  spawn,          RUNCMD("kitty") },
	{ MODKEY|ControlMask,   XK_d,                       spawn,          {.v = dmenucmd } },
	{ MODKEY,               XK_d,                       spawn,          {.v = gtk_dmenucmd } },
	
    { MODKEY,           	XK_F2,                      spawn,          RUNCMD("firefox") },
	{ MODKEY,        	    XK_z,                       spawn,          RUNCMD("zoom") },
	{ MODKEY,               XK_w,                       spawn,          RUNCMD("whatsapp-nativefier") },
	{ MODKEY|ShiftMask,     XK_d,                       spawn,          RUNCMD("discord") },
    { MODKEY,               XF86XK_AudioPlay,           spawn,          {.v = sptcmd} },
	{ MODKEY,               XK_s,                       spawn,          {.v = signalcmd } },

	{ MODKEY,        	    XK_r,                       spawn,          RUNCMD("passmenu") },

	{ MODKEY|ShiftMask,     XK_p,                       spawn,          RUNCMD("pavucontrol") },
	{ MODKEY,        	    XK_e,                       spawn,          RUNCMD("thunar") },
	{ MODKEY|ShiftMask,     XK_Return,                  zoom,           {0} },

    { MODKEY|ControlMask,   XK_s,                       spawn,          SHCMD("maim -s > $( find ~/gdrive -type d | dmenu )/$(date +%m-%d-%H-%M).png") },
    { MODKEY|ShiftMask,     XK_s,                       spawn,          SHCMD("maim -s | copyq write image/png - && copyq select 0") },
    { MODKEY,               XK_f,                       spawn,          SHCMD("feh --randomize personal/wallpaper --bg-fill --no-fehbg") },
    { MODKEY,               XK_period,                  spawn,          RUNCMD("emoji-chooser") },
    { MODKEY,               XK_v,                       spawn,          {.v = copymenu} },

    { MODKEY,               XK_b,                       togglebar,      {0} },
	{ MODKEY,               XK_j,                       focusstack,     {.i = +1 } },
	{ MODKEY,               XK_k,                       focusstack,     {.i = -1 } },
	{ MODKEY,               XK_i,                       incnmaster,     {.i = +1 } },
	{ MODKEY,               XK_o,                       incnmaster,     {.i = -1 } },
	{ MODKEY,               XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY,               XK_l,                       setmfact,       {.f = +0.05} },
	
    { MODKEY,               XK_t,                       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,               XK_x,                       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,               XK_m,                       setlayout,      {.v = &layouts[2]} },

    { MODKEY,               XK_Tab,                     view,           {0} },
    { MODKEY|ShiftMask,     XK_l,                       spawn,          RUNCMD("slock") },
	{ MODKEY|ShiftMask,     XK_q,                       killclient,     {0} },
 
    { MODKEY,               XK_Menu,                    spawn,          SHCMD("killall trayer; trayer --expand true --widthtype request --tint 0 --edge bottom --align right --height 40 --iconspacing 4&") },
    { MODKEY|ShiftMask,     XK_Menu,                    spawn,          SHCMD("killall trayer") },

    { 0,                    XF86XK_AudioLowerVolume,    spawn,          SHCMD("pamixer -d 5 && audio-notify") },
    { 0,                    XF86XK_AudioRaiseVolume,    spawn,          SHCMD("pamixer -i 5 && audio-notify") },
    { 0,                    XF86XK_AudioMute,           spawn,          SHCMD("pamixer -t && audio-notify") },
    { 0,                    XF86XK_AudioNext,           spawn,          SHCMD("playerctl -a next") },
    { 0,                    XF86XK_AudioPlay,           spawn,          SHCMD("playerctl -a play-pause") },
    { 0,                    XF86XK_AudioPrev,           spawn,          SHCMD("playerctl -a previous") },
	
    { MODKEY,               XK_Delete,                  spawn,          RUNCMD("networkmanager_dmenu") },
    { MODKEY,               XK_Insert,                  spawn,          RUNCMD("bluetoothctl_dmenu") },

    TAGKEYS(                XK_1,                       0)
	TAGKEYS(                XK_2,                       1)
	TAGKEYS(                XK_3,                       2)
	TAGKEYS(                XK_4,                       3)
	TAGKEYS(                XK_5,                       4)
	TAGKEYS(                XK_6,                       5)
	TAGKEYS(                XK_7,                       6)
	TAGKEYS(                XK_8,                       7)
	TAGKEYS(                XK_9,                       8)
    
    { MODKEY|ShiftMask,     XK_x,                       spawn,          SHCMD("systemctl suspend") },
    { MODKEY|ControlMask,   XK_x,                       spawn,          SHCMD("systemctl hybrid-sleep") },
    
    { MODKEY|ShiftMask,     XK_c,                       quit,           {.i = EXIT_SUCCESS} },
    { MODKEY|ControlMask,   XK_c,                       quit,           {.i = EXIT_FAILURE} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask          button          function        argument */
	{ ClkLtSymbol,          0,                  Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                  Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,                  Button1,        spawn,          {.v = calccmd } },
	{ ClkStatusText,        0,                  Button3,        spawn,          RUNCMD("pavucontrol") },
	{ ClkClientWin,         MODKEY,             Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,             Button3,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,   Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,                  Button1,        view,           {0} },
	{ ClkTagBar,            0,                  Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,             Button3,        toggletag,      {0} },
};

