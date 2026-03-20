stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r25, 0x14(r1)
mr r31, r4
cmplwi r3, 0x0
lis r4, lbl_80268B88@ha
addi r29, r4, lbl_80268B88@l
bne @8007CBE0
li r3, 0xa6
bl fn_80104704
@8007CBE0
bl fn_801040A0
lwz r28, 0x0(r3)
li r0, 0x0
li r30, 0x0
cmplwi r28, 0x0
stw r0, 0x4c(r31)
beq @8007D4E8
mr r3, r31
bl fn_801091F4
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8007D4E8
lha r0, 0x6(r31)
addi r6, r29, 0x0
mr r5, r6
li r4, 0x0
clrlwi r0, r0, 16
li r3, 0x4
mtctr r3
@8007CC2C
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CC3C
b @8007CD0C
@8007CC3C
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CC54
b @8007CD0C
@8007CC54
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CC6C
b @8007CD0C
@8007CC6C
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CC84
b @8007CD0C
@8007CC84
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CC9C
b @8007CD0C
@8007CC9C
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CCB4
b @8007CD0C
@8007CCB4
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CCCC
b @8007CD0C
@8007CCCC
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CCE4
b @8007CD0C
@8007CCE4
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CCFC
b @8007CD0C
@8007CCFC
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @8007CC2C
li r4, -0x1
@8007CD0C
cmpwi r4, 0x0
mr r7, r4
blt @8007CD24
li r0, 0x0
li r26, 0x0
b @8007D24C
@8007CD24
li r4, 0x0
li r3, 0x4
mtctr r3
@8007CD30
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CD40
b @8007CE10
@8007CD40
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CD58
b @8007CE10
@8007CD58
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CD70
b @8007CE10
@8007CD70
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CD88
b @8007CE10
@8007CD88
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CDA0
b @8007CE10
@8007CDA0
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CDB8
b @8007CE10
@8007CDB8
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CDD0
b @8007CE10
@8007CDD0
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CDE8
b @8007CE10
@8007CDE8
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CE00
b @8007CE10
@8007CE00
addi r6, r6, 0x2
addi r4, r4, 0x1
bdnz @8007CD30
li r4, -0x1
@8007CE10
cmpwi r4, 0x0
mr r7, r4
blt @8007CE28
li r0, 0x1
li r26, 0x0
b @8007D24C
@8007CE28
addi r6, r29, 0x120
li r4, 0x0
mr r5, r6
li r3, 0x4
mtctr r3
@8007CE3C
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CE4C
b @8007CF1C
@8007CE4C
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CE64
b @8007CF1C
@8007CE64
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CE7C
b @8007CF1C
@8007CE7C
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CE94
b @8007CF1C
@8007CE94
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CEAC
b @8007CF1C
@8007CEAC
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CEC4
b @8007CF1C
@8007CEC4
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CEDC
b @8007CF1C
@8007CEDC
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CEF4
b @8007CF1C
@8007CEF4
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007CF0C
b @8007CF1C
@8007CF0C
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @8007CE3C
li r4, -0x1
@8007CF1C
cmpwi r4, 0x0
mr r7, r4
blt @8007CF34
li r0, 0x0
li r26, 0x1
b @8007D24C
@8007CF34
li r4, 0x0
li r3, 0x4
mtctr r3
@8007CF40
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CF50
b @8007D020
@8007CF50
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CF68
b @8007D020
@8007CF68
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CF80
b @8007D020
@8007CF80
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CF98
b @8007D020
@8007CF98
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CFB0
b @8007D020
@8007CFB0
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CFC8
b @8007D020
@8007CFC8
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CFE0
b @8007D020
@8007CFE0
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007CFF8
b @8007D020
@8007CFF8
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D010
b @8007D020
@8007D010
addi r6, r6, 0x2
addi r4, r4, 0x1
bdnz @8007CF40
li r4, -0x1
@8007D020
cmpwi r4, 0x0
mr r7, r4
blt @8007D038
li r0, 0x1
li r26, 0x1
b @8007D24C
@8007D038
addi r6, r29, 0x90
li r4, 0x0
mr r5, r6
li r3, 0x4
mtctr r3
@8007D04C
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D05C
b @8007D12C
@8007D05C
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D074
b @8007D12C
@8007D074
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D08C
b @8007D12C
@8007D08C
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D0A4
b @8007D12C
@8007D0A4
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D0BC
b @8007D12C
@8007D0BC
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D0D4
b @8007D12C
@8007D0D4
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D0EC
b @8007D12C
@8007D0EC
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D104
b @8007D12C
@8007D104
addi r5, r5, 0x2
addi r4, r4, 0x1
lhz r3, 0x0(r5)
cmplw r0, r3
bne @8007D11C
b @8007D12C
@8007D11C
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @8007D04C
li r4, -0x1
@8007D12C
cmpwi r4, 0x0
mr r7, r4
blt @8007D144
li r0, 0x0
li r26, 0x2
b @8007D24C
@8007D144
li r4, 0x0
li r3, 0x4
mtctr r3
@8007D150
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D160
b @8007D230
@8007D160
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D178
b @8007D230
@8007D178
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D190
b @8007D230
@8007D190
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D1A8
b @8007D230
@8007D1A8
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D1C0
b @8007D230
@8007D1C0
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D1D8
b @8007D230
@8007D1D8
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D1F0
b @8007D230
@8007D1F0
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D208
b @8007D230
@8007D208
addi r6, r6, 0x2
addi r4, r4, 0x1
lhz r3, 0x48(r6)
cmplw r0, r3
bne @8007D220
b @8007D230
@8007D220
addi r6, r6, 0x2
addi r4, r4, 0x1
bdnz @8007D150
li r4, -0x1
@8007D230
cmpwi r4, 0x0
mr r7, r4
blt @8007D4E8
li r0, 0x1
li r26, 0x2
b @8007D24C
b @8007D4E8
@8007D24C
lwz r3, 0xac(r28)
cmpwi r3, 0x0
ble @8007D26C
slwi r3, r0, 2
add r3, r28, r3
lwz r3, 0xa0(r3)
cmpwi r3, 0x0
bge @8007D274
@8007D26C
li r3, 0x0
b @8007D280
@8007D274
lwz r4, 0xb0(r28)
slwi r3, r3, 2
lwzx r3, r4, r3
@8007D280
cmplwi r3, 0x0
beq @8007D4E8
lis r5, 0x2aab
lbz r4, 0x1c(r3)
subi r5, r5, 0x5555
mulhw r6, r5, r7
extsb r4, r4
srwi r5, r6, 31
add r5, r6, r5
mulli r6, r5, 0x6
extsb r27, r5
add r5, r3, r27
cmpw r27, r4
subf r4, r6, r7
lbz r25, 0x1e(r5)
extsb r6, r4
bge @8007D4E8
lbz r4, 0x1d(r3)
extsb r4, r4
cmpw r6, r4
bne @8007D358
add r4, r28, r0
lbz r4, 0xb4(r4)
bl fn_80082FE4
cmpwi r26, 0x1
beq @8007D31C
bge @8007D2F8
cmpwi r26, 0x0
bge @8007D304
b @8007D4B8
@8007D2F8
cmpwi r26, 0x3
bge @8007D4B8
b @8007D34C
@8007D304
slwi r0, r25, 2
addi r3, r29, 0x1d0
lwzx r3, r3, r0
bl fn_8005D858
mr r30, r3
b @8007D4B8
@8007D31C
mulli r5, r27, 0xe
add r4, r3, r5
lbz r0, 0x1c(r4)
cmplwi r0, 0x0
beq @8007D4B8
li r0, 0xe5
addi r4, r5, 0x10
stw r0, 0x4c(r31)
add r4, r3, r4
li r3, 0x37
bl fn_80132A38
b @8007D4B8
@8007D34C
li r0, 0x3ccf
stw r0, 0x4c(r31)
b @8007D4B8
@8007D358
bge @8007D4E8
add r4, r28, r0
mr r5, r27
lbz r4, 0xb4(r4)
bl fn_80082EA4
mr r27, r3
lbz r0, 0xc(r27)
cmplwi r0, 0x0
beq @8007D388
mr r4, r27
li r3, 0x37
bl fn_80132A38
@8007D388
cmpwi r26, 0x1
beq @8007D3C4
bge @8007D3A0
cmpwi r26, 0x0
bge @8007D3AC
b @8007D4B8
@8007D3A0
cmpwi r26, 0x3
bge @8007D4B8
b @8007D3E8
@8007D3AC
slwi r0, r25, 2
addi r3, r29, 0x1b0
lwzx r3, r3, r0
bl fn_8005D858
mr r30, r3
b @8007D4B8
@8007D3C4
lbz r0, 0xc(r27)
cmplwi r0, 0x0
beq @8007D4B8
li r0, 0xe5
mr r4, r27
stw r0, 0x4c(r31)
li r3, 0x37
bl fn_80132A38
b @8007D4B8
@8007D3E8
lbz r0, 0xc(r27)
cmplwi r0, 0x0
beq @8007D4B8
li r0, 0xe5
lis r4, 0x51ec
stw r0, 0x4c(r31)
lis r3, 0x6666
subi r5, r4, 0x7ae1
li r7, lbl_8047A658@sda21
lhz r0, 0xe(r27)
addi r8, r3, 0x6667
li r4, 0x0
li r3, 0x37
mulhw r5, r5, r0
sth r4, 0x6(r7)
li r4, lbl_8047A658@sda21
srawi r5, r5, 5
srwi r6, r5, 31
add r12, r5, r6
mulhw r5, r8, r12
srawi r5, r5, 2
mulhw r9, r8, r0
srwi r6, r5, 31
add r11, r5, r6
srawi r5, r9, 2
srwi r6, r5, 31
add r10, r5, r6
mulhw r5, r8, r10
srawi r8, r5, 2
srawi r5, r9, 2
srwi r9, r8, 31
srwi r6, r5, 31
add r8, r8, r9
add r5, r5, r6
mulli r9, r11, 0xa
mulli r6, r8, 0xa
subf r8, r9, r12
mulli r5, r5, 0xa
addi r8, r8, 0x30
subf r6, r6, r10
clrlwi r8, r8, 16
subf r5, r5, r0
addi r6, r6, 0x30
addi r0, r5, 0x30
sth r8, lbl_8047A658@sda21(r0)
clrlwi r5, r6, 16
clrlwi r0, r0, 16
sth r5, 0x2(r7)
sth r0, 0x4(r7)
bl fn_80132A38
b @8007D4B8
b @8007D4E8
@8007D4B8
cmplwi r30, 0x0
beq @8007D4E8
lwz r0, 0x10(r30)
stw r0, 0x58(r31)
lha r0, 0x8(r30)
sth r0, 0x5c(r31)
lha r0, 0xa(r30)
sth r0, 0x5e(r31)
lha r0, 0xc(r30)
sth r0, 0x60(r31)
lha r0, 0xe(r30)
sth r0, 0x62(r31)
@8007D4E8
lmw r25, 0x14(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
