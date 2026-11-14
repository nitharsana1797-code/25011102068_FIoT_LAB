$NOMOD51
; Generic 8051 startup file for Keil A51
; Initializes stack pointer and transfers control to C runtime.
NAME    ?C_STARTUP
?C_STARTUP SEGMENT CODE
RSEG    ?C_STARTUP
        LJMP    STARTUP1
        ORG     0000H
        LJMP    STARTUP1
        ORG     0003H
        RETI
        ORG     000BH
        RETI
        ORG     0013H
        RETI
        ORG     001BH
        RETI
        ORG     0023H
        RETI
STARTUP1:
        MOV     SP,#07H
        LJMP    ?C_START
END
