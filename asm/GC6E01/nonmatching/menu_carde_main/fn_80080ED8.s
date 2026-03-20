cmplwi r4, 0x0
li r5, 0x0
bne @80080EEC
li r3, 0x0
blr
@80080EEC
cmplwi r3, 0x0
bne @80081010
b @80080F34
@80080EF8
cmplwi r3, 0x81
blt @80080F0C
clrlwi r0, r3, 24
cmplwi r0, 0x9f
ble @80080F20
@80080F0C
clrlwi r0, r3, 24
cmplwi r0, 0xe0
blt @80080F2C
cmplwi r0, 0xfc
bgt @80080F2C
@80080F20
addi r4, r4, 0x2
addi r5, r5, 0x2
b @80080F34
@80080F2C
addi r4, r4, 0x1
addi r5, r5, 0x1
@80080F34
lbz r3, 0x0(r4)
cmplwi r3, 0x0
bne @80080EF8
b @80081024
b @80081010
@80080F48
cmplwi r6, 0x81
blt @80080F5C
clrlwi r0, r6, 24
cmplwi r0, 0x9f
ble @80080F70
@80080F5C
clrlwi r0, r6, 24
cmplwi r0, 0xe0
blt @80080FDC
cmplwi r0, 0xfc
bgt @80080FDC
@80080F70
cmplwi r6, 0x81
blt @80080FAC
cmplwi r6, 0x9f
bgt @80080FAC
mulli r7, r6, 0x170
lbz r0, 0x1(r4)
lis r6, lbl_80269B68@ha
slwi r0, r0, 1
addi r6, r6, lbl_80269B68@l
add r6, r6, r7
add r6, r6, r0
subis r6, r6, 0x1
lhz r0, 0x4610(r6)
mr r6, r0
b @80080FD4
@80080FAC
mulli r7, r6, 0x170
lbz r0, 0x1(r4)
lis r6, lbl_8026C7F8@ha
slwi r0, r0, 1
addi r6, r6, lbl_8026C7F8@l
add r6, r6, r7
add r6, r6, r0
subis r6, r6, 0x1
lhz r0, -0x4280(r6)
mr r6, r0
@80080FD4
li r0, 0x2
b @80081000
@80080FDC
clrlwi r0, r6, 24
cmplwi r0, 0xa1
blt @80080FFC
cmplwi r0, 0xdf
bgt @80080FFC
addis r6, r6, 0x1
subi r0, r6, 0x140
clrlwi r6, r0, 16
@80080FFC
li r0, 0x1
@80081000
sth r6, 0x0(r3)
addi r3, r3, 0x2
add r4, r4, r0
add r5, r5, r0
@80081010
lbz r6, 0x0(r4)
cmplwi r6, 0x0
bne @80080F48
li r0, 0x0
sth r0, 0x0(r3)
@80081024
mr r3, r5
blr
