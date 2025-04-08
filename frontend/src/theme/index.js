import { createContext, useState, useMemo } from "react";
import { createTheme } from "@mui/material/styles";

// 색상 디자인 토큰
export const tokens = (mode) => ({
  ...(mode === "dark"
    ? {
        primary: {
          100: "#d0e7ff",
          200: "#a0cfff",
          300: "#71b7ff",
          400: "#419fff",
          500: "#1287ff",
          600: "#0e6ccc",
          700: "#0a5199",
          800: "#073666",
          900: "#031b33"
        },
        secondary: {
          100: "#fff9c4",
          200: "#fff59d",
          300: "#fff176",
          400: "#ffee58",
          500: "#ffeb3b",
          600: "#fdd835",
          700: "#fbc02d",
          800: "#f9a825",
          900: "#f57f17"
        },
        success: {
          100: "#d4edda",
          200: "#a9dbb6",
          300: "#7fc991",
          400: "#54b76d",
          500: "#28a548",
          600: "#20843a",
          700: "#18632b",
          800: "#10421d",
          900: "#08210e"
        },
        warning: {
          100: "#fff7cc",
          200: "#ffef99",
          300: "#ffe666",
          400: "#ffde33",
          500: "#ffd600",
          600: "#ccab00",
          700: "#998000",
          800: "#665600",
          900: "#332b00"
        },
        error: {
          100: "#ffcdd2",
          200: "#ef9a9a",
          300: "#e57373",
          400: "#ef5350",
          500: "#f44336",
          600: "#e53935",
          700: "#d32f2f",
          800: "#c62828",
          900: "#b71c1c"
        },
        grey: {
          100: "#e0e0e0",
          200: "#c2c2c2",
          300: "#a3a3a3",
          400: "#858585",
          500: "#666666",
          600: "#525252",
          700: "#3d3d3d",
          800: "#292929",
          900: "#141414"
        },
        background: {
          default: "#071525",
          paper: "#0a1929"
        },
        text: {
          primary: "#ffffff",
          secondary: "#b0bec5",
          disabled: "#546e7a"
        },
        action: {
          active: "#90caf9",
          hover: "rgba(144, 202, 249, 0.08)",
          selected: "rgba(144, 202, 249, 0.16)",
          disabled: "rgba(144, 202, 249, 0.3)",
          disabledBackground: "rgba(144, 202, 249, 0.12)"
        },
        divider: "#1e3a5a"
      }
    : {
        primary: {
          100: "#031b33",
          200: "#073666",
          300: "#0a5199",
          400: "#0e6ccc",
          500: "#1287ff",
          600: "#419fff",
          700: "#71b7ff",
          800: "#a0cfff",
          900: "#d0e7ff"
        },
        secondary: {
          100: "#f57f17",
          200: "#f9a825",
          300: "#fbc02d",
          400: "#fdd835",
          500: "#ffeb3b",
          600: "#ffee58",
          700: "#fff176",
          800: "#fff59d",
          900: "#fff9c4"
        },
        success: {
          100: "#08210e",
          200: "#10421d",
          300: "#18632b",
          400: "#20843a",
          500: "#28a548",
          600: "#54b76d",
          700: "#7fc991",
          800: "#a9dbb6",
          900: "#d4edda"
        },
        warning: {
          100: "#332b00",
          200: "#665600",
          300: "#998000",
          400: "#ccab00",
          500: "#ffd600",
          600: "#ffde33",
          700: "#ffe666",
          800: "#ffef99",
          900: "#fff7cc"
        },
        error: {
          100: "#b71c1c",
          200: "#c62828",
          300: "#d32f2f",
          400: "#e53935",
          500: "#f44336",
          600: "#ef5350",
          700: "#e57373",
          800: "#ef9a9a",
          900: "#ffcdd2"
        },
        grey: {
          100: "#141414",
          200: "#292929",
          300: "#3d3d3d",
          400: "#525252",
          500: "#666666",
          600: "#858585",
          700: "#a3a3a3",
          800: "#c2c2c2",
          900: "#e0e0e0"
        },
        background: {
          default: "#f8f9fa",
          paper: "#ffffff"
        },
        text: {
          primary: "#263238",
          secondary: "#546e7a",
          disabled: "#b0bec5"
        },
        action: {
          active: "#0d47a1",
          hover: "rgba(13, 71, 161, 0.08)",
          selected: "rgba(13, 71, 161, 0.16)",
          disabled: "rgba(13, 71, 161, 0.3)",
          disabledBackground: "rgba(13, 71, 161, 0.12)"
        },
        divider: "#e0e0e0"
      }),
});

// mui 테마 설정
export const themeSettings = (mode) => {
  const colors = tokens(mode);

  return {
    palette: {
      mode: mode,
      ...(mode === "dark"
        ? {
            primary: {
              main: colors.primary[500],
            },
            secondary: {
              main: colors.secondary[500],
            },
            neutral: {
              dark: colors.grey[700],
              main: colors.grey[500],
              light: colors.grey[100],
            },
            background: {
              default: colors.background.default,
              paper: colors.background.paper,
            },
          }
        : {
            primary: {
              main: colors.primary[500],
            },
            secondary: {
              main: colors.secondary[500],
            },
            neutral: {
              dark: colors.grey[700],
              main: colors.grey[500],
              light: colors.grey[100],
            },
            background: {
              default: colors.background.default,
              paper: colors.background.paper,
            },
          }),
    },
    typography: {
      fontFamily: ["Roboto", "Noto Sans KR", "sans-serif"].join(","),
      fontSize: 12,
      h1: {
        fontFamily: ["Roboto", "Noto Sans KR", "sans-serif"].join(","),
        fontSize: 40,
      },
      h2: {
        fontFamily: ["Roboto", "Noto Sans KR", "sans-serif"].join(","),
        fontSize: 32,
      },
      h3: {
        fontFamily: ["Roboto", "Noto Sans KR", "sans-serif"].join(","),
        fontSize: 24,
      },
      h4: {
        fontFamily: ["Roboto", "Noto Sans KR", "sans-serif"].join(","),
        fontSize: 20,
      },
      h5: {
        fontFamily: ["Roboto", "Noto Sans KR", "sans-serif"].join(","),
        fontSize: 16,
      },
      h6: {
        fontFamily: ["Roboto", "Noto Sans KR", "sans-serif"].join(","),
        fontSize: 14,
      },
    },
    components: {
      MuiButton: {
        styleOverrides: {
          root: {
            textTransform: 'none',
            borderRadius: 4,
          },
        },
      },
      MuiPaper: {
        styleOverrides: {
          root: {
            backgroundImage: 'none',
          },
        },
      },
      MuiCard: {
        styleOverrides: {
          root: {
            backgroundImage: 'none',
            borderRadius: 8,
            boxShadow: mode === 'dark' 
              ? '0 8px 16px rgba(0, 0, 0, 0.4)' 
              : '0 4px 12px rgba(0, 0, 0, 0.05)',
          },
        },
      },
      MuiTableCell: {
        styleOverrides: {
          root: {
            borderColor: mode === 'dark' ? colors.divider : colors.divider,
          },
        },
      },
    },
  };
};

// 컨텍스트 설정
export const ColorModeContext = createContext({
  toggleColorMode: () => {},
});

export const useMode = () => {
  const [mode, setMode] = useState("dark");

  const colorMode = useMemo(
    () => ({
      toggleColorMode: () =>
        setMode((prev) => (prev === "light" ? "dark" : "light")),
    }),
    []
  );

  const theme = useMemo(() => createTheme(themeSettings(mode)), [mode]);

  return [theme, colorMode];
}; 