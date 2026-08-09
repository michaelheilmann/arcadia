// The author of this software is Michael Heilmann (contact@michaelheilmann.com).
//
// Copyright(c) 2024-2025 Michael Heilmann (contact@michaelheilmann.com).
//
// Permission to use, copy, modify, and distribute this software for any
// purpose without fee is hereby granted, provided that this entire notice
// is included in all copies of any software which is or includes a copy
// or modification of this software and in all copies of the supporting
// documentation for such software.
//
// THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY.IN PARTICULAR, NEITHER THE AUTHOR NOR LUCENT MAKES ANY
// REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
// OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.

// Keep the entries in this file in alphabetical order.
// Key names must be unique. If two keys names have the same number, then they are aliases of each other.

Define(A, 65, "A")
Define(Accept, 30, "IME Accept")
Define(Add, 107, "Add")
Define(Applications, 93, "Applications")


Define(B, 66, "B")
Define(Backspace, 1, "Backspace")


Define(BrowserBack, 166, "Browser back")
Define(BrowserFavorites, 171, "Browser favorites")
Define(BrowserForward, 167, "Browser forward")
Define(BrowserHome, 172, "Browser home")
Define(BrowserRefresh, 168, "Browser refresh")
Define(BrowserSearch, 170, "Browser search")
Define(BrowserStop, 169, "Browser stop")


Define(C, 67, "C")
#if defined(withAliases)
Define(Capital, 6, "Capital")											// "Capital" key. Alias of "CapsLock" key.
#endif
Define(CapsLock, 6, "Caps lock")										// "CapsLock"" key. Alias of "Capital" key.
Define(Clear, 12, "Clear")
Define(Comma, 188, "Comma")
Define(Convert, 28, "IME convert")


Define(D, 68, "D")
Define(Zero, 48, "0")
Define(One, 49, "1")
Define(Two, 50, "2")
Define(Three, 51, "3")
Define(Four, 52, "4")
Define(Five, 53, "5")
Define(Six, 54, "6")
Define(Seven, 55, "7")
Define(Eight, 56, "8")
Define(Nine, 57, "9")
Define(DeadCircumflex, 221, "Dead Circumflex")                          // "^" aka "VK_OEM_5" in WinAPI.
Define(DeadGrave, 219, "Dead Grave")                                    // "`" aka "VK_OEM_6" in WinAPI.
Define(Decimal, 110, "Decimal")											// Decimal separator on NumPad
Define(Delete, 46, "Delete")
Define(Divide, 111, "Divide")
#if defined(withAliases)
Define(Down, 40, "Down")												// "Down" key. Alias of "Down Arrow" key.
#endif
Define(DownArrow, 40, "Down Arrow")										// "Down Arrow" key. Alias of "Down" key.


Define(E, 69, "E")
Define(End, 61, "End")
Define(Escape, 8, "Escape")


Define(F, 70, "F")
Define(F1, 112, "F1")
Define(F10, 121, "F10")
Define(F11, 122, "F11")
Define(F12, 123, "F12")
Define(F13, 124, "F13")
Define(F14, 125, "F14")
Define(F15, 126, "F15")
Define(F16, 127, "F16")
Define(F17, 128, "F17")
Define(F18, 129, "F18")
Define(F19, 130, "F19")
Define(F2, 113, "F2")
Define(F20, 131, "F20")
Define(F21, 132, "F21")
Define(F22, 133, "F22")
Define(F23, 134, "F23")
Define(F24, 135, "F24")
Define(F3, 114, "F3")
Define(F4, 115, "F4")
Define(F5, 116, "F5")
Define(F6, 117, "F6")
Define(F7, 118, "F7")
Define(F8, 119, "F8")
Define(F9, 120, "F9")
Define(FinalMode, 24, "IME Final Mode")


Define(G, 71, "G")


Define(H, 72, "H")
Define(HangulMode, 21, "IME Hangul Mode")								// "IME Hangul Mode" key. Alias of "IME Hanguel Mode" key.
Define(HanjaMode, 25, "IME Hanja Mode")								    // "IME Hanja Mode"" key. Alias of "IME Kanji Mode" key.
Define(Home, 36, "Home")


Define(I, 73, "I")
Define(Insert, 58, "Insert")


Define(J, 74, "J")
Define(JunjaMode, 23, "IME Junja Mode") // "IME Junja Mode"" key.


Define(K, 75, "K")
#if defined(withAliases)
Define(KanaMode, 21, "IME Kana Mode") // "IME Kana Mode" key. Alias of "IME Hangul Mode" key.
#endif
#if defined(withAliases)
Define(KanjiMode, 25, "IME Kanji Mode") // "IME Kanji Mode" key. Alias of "IME Hanja Mode" key.
#endif

Define(L, 76, "L")
#if defined(withAliases)
Define(Left, 37," Left") // "Left" key. Alias of "Left Arrow" key.
#endif
Define(LeftArrow, 37, "Left Arrow")	// "Left Arrow" key. Alias of "Left" key.
Define(LeftControl, 162, "Left Control")
Define(LeftMenu, 154, "Left Menu") // "Left Menu" aka "Left Alt".
Define(LeftShift, 160, "Left Shift")
Define(LeftWindows, 91, "Left Windows")
Define(Less, 226, "Less")


Define(M, 77, "M")
Define(Minus, 189, "Minus")
Define(ModeChange, 31, "IME Mode Change Request")
Define(Multiply, 106, "Multiply")

Define(N, 78, "N")
Define(NonConvert, 29, "IME NonConvert")
Define(NumberSign, 227, "Number Sign") // "#" aka "VK_OEM_2" in WinAPI.
Define(NumLock, 144, "Num Lock")
Define(NumPadZero, 96, "NumPad 0")
Define(NumPadOne, 97, "NumPad 1")
Define(NumPadTwo, 98, "NumPad 2")
Define(NumPadThree, 99, "NumPad 3")
Define(NumPadFour, 100, "NumPad 4")
Define(NumPadFive, 101, "NumPad 5")
Define(NumPadSix, 102, "NumPad 6")
Define(NumPadSeven, 103, "NumPad 7")
Define(NumPadEight, 104, "NumPad 8")
Define(NumPadNine, 105, "NumPad 9")
Define(NumPadEnter, 108, "NumPad Enter")


Define(O, 79, "O")


Define(P, 80, "P")
Define(PageDown, 34, "Page Down")
Define(PageUp, 33, "Page Up")
Define(Pause, 19, "Pause")
Define(Period, 190, "Period")
Define(Print, 42, "Print")
Define(Plus, 187, "Plus")


Define(Q, 81, "Q")


Define(R, 82, "R")
Define(Return, 13, "Return")
#if defined(withAliases)
Define(Right, 39, "Right")
#endif
Define(RightArrow, 39, "RightArrow")
Define(RightControl, 14, "Right Control")
Define(RightMenu, 16, "Right Menu")  // "Right Menu" aka "Right Alt".
Define(RightShift, 11, "Right Shift")
Define(RightWindows, 92, "Right Windows")


Define(S, 83, "S")
Define(ScrollLock, 145, "Scroll Lock")
Define(Sleep, 95, "Computer Sleep")
Define(Space, 32, "Space")
Define(Subtract, 109, "Subtract")


Define(T, 84, "T")
Define(Tabulator, 9, "Tabulator")


Define(U, 85, "U")
#if defined(withAliases)
Define(Up, 38, "Up")													// "Up" key. Alias of "Up Arrow" key.
#endif
Define(UpArrow, 38, "Up Arrow")											// "Up Arrow" key. Alias of "Up" key.


Define(V, 86, "V")


Define(W, 87, "W")


Define(X, 88, "X")


Define(Y, 89, "Y")


Define(Z, 90, "Z")
