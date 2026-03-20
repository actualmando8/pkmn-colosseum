stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r25, 0x14(r1)
mr r28, r3
bl fn_801EF634
mr r27, r3
li r25, 0x0
li r3, 0x0
bl fn_80103CC0
li r3, 0xdf
li r4, 0x0
bl fn_8010264C
li r3, 0xba
li r4, 0x1
bl fn_8010264C
clrlwi r0, r27, 16
cmpwi r0, 0x1
beq @80062B04
b @80062CC8
@80062B04
li r3, 0x1
bl fn_80103CC0
bl fn_8025DA88
cmpwi r3, 0x2
bne @80062B3C
li r3, 0x0
bl fn_8025D9F0
clrlwi r0, r3, 16
cmpwi r0, 0x0
beq @80062B34
li r0, 0x1
b @80062B40
@80062B34
li r0, 0x0
b @80062B40
@80062B3C
li r0, 0x0
@80062B40
clrlwi r0, r0, 16
cmplwi r0, 0x0
bne @80062BEC
bl fn_8025DA3C
mr r29, r3
bl fn_8025D9A8
li r30, 0x0
b @80062BB0
@80062B60
mr r3, r30
bl fn_8025D9F0
mr r27, r3
mr r3, r30
bl fn_8025D2B0
mr r31, r3
cmpwi r31, 0x0
beq @80062BAC
clrlwi r0, r27, 16
cmplwi r0, 0x1
beq @80062B94
cmplwi r0, 0x2
bne @80062BAC
@80062B94
mr r3, r31
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80062BAC
b @80062BBC
@80062BAC
addi r30, r30, 0x1
@80062BB0
cmpw r30, r29
blt @80062B60
li r31, 0x2
@80062BBC
mr r4, r31
li r3, 0x30
bl fn_80132A38
li r3, 0x2
li r4, 0x44dc
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r25, 0x1
b @80062CC8
@80062BEC
bl fn_8025DA3C
mr r29, r3
bl fn_8025D9A8
li r30, 0x0
b @80062C44
@80062C00
mr r3, r30
bl fn_8025D9F0
mr r27, r3
mr r3, r30
bl fn_8025D2B0
cmpwi r3, 0x0
beq @80062C40
clrlwi r0, r27, 16
cmplwi r0, 0x1
beq @80062C30
cmplwi r0, 0x2
bne @80062C40
@80062C30
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80062C4C
@80062C40
addi r30, r30, 0x1
@80062C44
cmpw r30, r29
blt @80062C00
@80062C4C
li r3, 0x2
li r4, 0x44e7
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
li r27, 0x1
@80062C64
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80062C98
li r3, 0x1
bl fn_800F7C28
cmpwi r3, 0x0
bne @80062C90
li r0, 0x1
b @80062C9C
@80062C90
li r0, 0x0
b @80062C9C
@80062C98
li r0, 0x0
@80062C9C
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80062CB0
li r27, 0x0
b @80062CB4
@80062CB0
bl fn_800F0308
@80062CB4
cmpwi r27, 0x0
bne @80062C64
li r3, 0x1
bl fn_801069FC
li r25, 0x1
@80062CC8
clrlwi r0, r25, 24
cmplwi r0, 0x0
beq @80062CDC
li r3, 0xb3
b @8006304C
@80062CDC
lwz r0, 0x4(r28)
cmpwi r0, 0x2
beq @80062CF0
li r25, 0xd4
b @80062CF4
@80062CF0
li r25, 0xd5
@80062CF4
li r3, 0x1
bl fn_80103CC0
li r3, 0x2
li r4, 0x3c20
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
bl fn_801046B8
clrlwi r31, r25, 16
mr r4, r3
mr r3, r31
li r5, 0x0
li r6, 0x8
li r7, 0x1
li r8, 0x0
crclr 6
bl fn_801026A4
lwz r0, 0x4(r28)
cmpwi r0, 0x2
beq @80062D50
cmpwi r3, 0x0
ble @80062D50
addi r3, r3, 0x1
@80062D50
cmpwi r3, 0x1
beq @80062D84
bge @80062D6C
cmpwi r3, -0x1
beq @80062D94
bge @80062D78
b @80062D94
@80062D6C
cmpwi r3, 0x3
bge @80062D94
b @80062D8C
@80062D78
bl fn_8025D788
li r30, 0xd1
b @80062D98
@80062D84
li r30, 0xb5
b @80062D98
@80062D8C
li r30, 0xb3
b @80062D98
@80062D94
li r30, -0x1
@80062D98
li r3, 0x1
bl fn_801069FC
cmpwi r30, -0x1
beq @80062DB0
cmpwi r30, 0xb3
bne @80063040
@80062DB0
li r25, 0x0
li r29, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
bne @80062DE8
li r3, 0x0
bl fn_8025D9F0
clrlwi r0, r3, 16
cmpwi r0, 0x0
beq @80062DE0
li r0, 0x1
b @80062DEC
@80062DE0
li r0, 0x0
b @80062DEC
@80062DE8
li r0, 0x0
@80062DEC
clrlwi r28, r0, 16
@80062DF0
cmpwi r25, 0x2
beq @80062EEC
bge @80062E0C
cmpwi r25, 0x0
beq @80062E1C
bge @80062E34
b @80063038
@80062E0C
cmpwi r25, 0x4
beq @80063034
bge @80063038
b @80062FA4
@80062E1C
cmpwi r28, 0x0
bne @80062E2C
li r25, 0x1
b @80063038
@80062E2C
li r25, 0x2
b @80063038
@80062E34
mr r3, r31
li r4, 0x0
li r5, 0x1
bl fn_80102568
li r3, 0x2
li r4, 0x4446
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
li r25, 0x1
@80062E5C
li r26, 0x0
@80062E60
mr r3, r26
bl fn_8025D9F0
mr r27, r3
mr r3, r26
bl fn_8025D2B0
cmpwi r3, 0x0
beq @80062EA8
clrlwi r0, r27, 16
cmplwi r0, 0x1
beq @80062E90
cmplwi r0, 0x2
bne @80062EA8
@80062E90
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80062EA8
li r0, 0x0
b @80062EB8
@80062EA8
addi r26, r26, 0x1
cmpwi r26, 0x4
blt @80062E60
li r0, 0x1
@80062EB8
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80062EC8
li r25, 0x0
@80062EC8
cmpwi r25, 0x0
beq @80062ED4
bl fn_800F0308
@80062ED4
cmpwi r25, 0x0
bne @80062E5C
li r3, 0x1
bl fn_801069FC
li r25, 0x4
b @80063038
@80062EEC
mr r3, r31
li r4, 0x0
li r5, 0x1
bl fn_80102568
li r3, 0x2
li r4, 0x4445
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
li r25, 0x1
@80062F14
li r26, 0x0
@80062F18
mr r3, r26
bl fn_8025D9F0
mr r27, r3
mr r3, r26
bl fn_8025D2B0
cmpwi r3, 0x0
beq @80062F60
clrlwi r0, r27, 16
cmplwi r0, 0x1
beq @80062F48
cmplwi r0, 0x2
bne @80062F60
@80062F48
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80062F60
li r0, 0x0
b @80062F70
@80062F60
addi r26, r26, 0x1
cmpwi r26, 0x4
blt @80062F18
li r0, 0x1
@80062F70
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80062F80
li r25, 0x0
@80062F80
cmpwi r25, 0x0
beq @80062F8C
bl fn_800F0308
@80062F8C
cmpwi r25, 0x0
bne @80062F14
li r3, 0x1
bl fn_801069FC
li r25, 0x3
b @80063038
@80062FA4
mr r3, r31
li r4, 0x0
li r5, 0x1
bl fn_80102568
li r3, 0x2
li r4, 0x44e2
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
li r26, 0x1
@80062FCC
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80063000
li r3, 0x1
bl fn_800F7C28
cmpwi r3, 0x0
bne @80062FF8
li r0, 0x1
b @80063004
@80062FF8
li r0, 0x0
b @80063004
@80063000
li r0, 0x0
@80063004
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80063018
li r26, 0x0
b @8006301C
@80063018
bl fn_800F0308
@8006301C
cmpwi r26, 0x0
bne @80062FCC
li r3, 0x1
bl fn_801069FC
li r25, 0x4
b @80063038
@80063034
li r29, 0x0
@80063038
cmpwi r29, 0x0
bne @80062DF0
@80063040
li r0, 0x0
mr r3, r30
stw r0, lbl_8047A5D0@sda21(r0)
@8006304C
lmw r25, 0x14(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
