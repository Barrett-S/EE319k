; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
z equ 4	;offset
	SUB SP,#8 ;allocate
	MOV R1,#0	;clear digit amount
	MOV R12,SP
	MOV R3,#10	;local variable value
	STR R3,[R12,#z]
Loop3
	MOV R2,R0
	LDR R3,[R12,#z]	;take out powers of 10
	UDIV R0,R3
	MUL R3,R0,R3
	SUB R3,R2,R3
	PUSH {R3,R11}	;push digit values on stack
	ADD R1,#1		;increment digits
	CMP R0,#0
	BNE Loop3
Loop4
	POP {R0,R11}	;pop digit values off stack
	ADD R0,#48		;convert digit values to Ascii
	PUSH {R1,LR}
	BL ST7735_OutChar
	POP {R1,LR}
	SUB R1,#1	;decrement digits
	CMP R1,#0
	BNE Loop4
	ADD SP,#8 ;deallocate
    BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
x equ 0
	PUSH {R11,LR}
	SUB SP,#8
	MOV R11,SP
	MOV R1,#9999
	CMP R0,R1
	BHI High
	MOV R1,#999
	CMP R0,R1
	BHI Thousand
	CMP R0,#99
	BHI Hundred
	CMP R0,#9
	BHI Ten
	PUSH {R0,LR}
	MOV R0,#0x30
	BL ST7735_OutChar
	MOV R0,#0x2E
	BL ST7735_OutChar
	MOV R0,#0x30
	BL ST7735_OutChar
	MOV R0,#0x30
	BL ST7735_OutChar
	POP {R0,LR}
	BL LCD_OutDec
	ADD SP,#8
	POP {R11, LR}
	BX LR
Thousand
	PUSH {R0,LR}
	MOV R1,#1000
	UDIV R0,R1
	MUL R2,R0,R1
	STR R2,[R11,#x]
	BL LCD_OutDec
	MOV R0,#0x2E
	BL ST7735_OutChar
	POP {R0,LR}
	LDR R2,[R11,#x]
	SUB R0,R2
	CMP R0,#99
	BHI Hundred1
	CMP R0,#9
	BHI Ten1
	PUSH {R0,LR}
	MOV R0,#0x30
	BL ST7735_OutChar
	MOV R0,#0x30
	BL ST7735_OutChar
	POP {R0,LR}
	BL LCD_OutDec
	ADD SP,#8
	POP {R11, LR}
	BX LR
Hundred
	PUSH {R0,LR}
	MOV R0,#0x30
	BL ST7735_OutChar
	MOV R0,#0x2E
	BL ST7735_OutChar
	POP {R0,LR}
Hundred1
	BL LCD_OutDec
	ADD SP,#8
	POP {R11, LR}
	BX LR
Ten
	PUSH {R0,LR}
	MOV R0,#0x30
	BL ST7735_OutChar
	MOV R0,#0x2E
	BL ST7735_OutChar
Ten1
	MOV R0,#0x30
	BL ST7735_OutChar
	POP {R0,LR}
	BL LCD_OutDec
	ADD SP,#8
	POP {R11, LR}
	BX LR
High
	MOV R0,#0x2A
	BL ST7735_OutChar
	MOV R0,#0x2E
	BL ST7735_OutChar
	MOV R0,#0x2A
	BL ST7735_OutChar
	MOV R0,#0x2A
	BL ST7735_OutChar
	MOV R0,#0x2A
	BL ST7735_OutChar
	ADD SP,#8
	POP {R11,LR}
     BX   LR
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
