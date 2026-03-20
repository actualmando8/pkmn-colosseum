stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
mr r30, r3
mr r26, r4
li r3, 0x0
bl fn_80083CFC
mr r31, r3
cmplwi r31, 0x0
bne @8008971C
li r3, 0x0
b @80089964
@8008971C
lbz r0, 0x4000(r31)
cmplwi r0, 0x1
bne @80089748
addi r4, r31, 0x4004
li r3, 0x4d
bl fn_80132A38
mr r3, r30
mr r4, r26
li r5, 0xe0
bl fn_80189990
b @8008978C
@80089748
addi r4, r31, 0x4060
li r3, 0x4d
bl fn_80132A38
mr r3, r30
mr r4, r26
li r5, 0xe0
bl fn_80189990
bl fn_8001E184
extsb r0, r3
cmpwi r0, 0x0
beq @8008978C
bge @80089780
cmpwi r0, -0x1
b @80089784
@80089780
cmpwi r0, 0x2
@80089784
li r3, 0x0
b @80089964
@8008978C
li r0, 0x2
li r3, 0x231
stb r0, 0x4000(r31)
bl fn_8020E0F8
li r3, 0x9
bl fn_801FCCC4
lbz r4, 0x4124(r31)
mr r28, r3
bl fn_801FCB94
lbz r4, 0x4125(r31)
mr r3, r28
bl fn_801FCC54
lhz r4, 0x4134(r31)
mr r3, r28
bl fn_801FCB84
lbz r4, 0x4136(r31)
mr r3, r28
bl fn_801FCAFC
mr r27, r31
li r29, 0x0
@800897DC
lhz r5, 0x4126(r27)
mr r3, r28
clrlwi r4, r29, 24
bl fn_801FCB40
addi r27, r27, 0x2
addi r29, r29, 0x1
cmpwi r29, 0x4
blt @800897DC
mr r3, r28
bl fn_801FCC3C
bl fn_801FCA2C
li r29, 0x0
li r26, 0x0
mulli r0, r29, 0x2a
mr r28, r3
add r27, r31, r0
addi r27, r27, 0x4000
b @8008984C
@80089824
mr r3, r28
addi r4, r27, 0x138
bl fn_80089978
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @80089844
addi r28, r28, 0x50
addi r29, r29, 0x1
@80089844
addi r27, r27, 0x2a
addi r26, r26, 0x1
@8008984C
cmpwi r26, 0x4
blt @80089824
b @8008986C
@80089858
mr r3, r28
li r4, 0x0
bl fn_801FC794
addi r28, r28, 0x50
addi r29, r29, 0x1
@8008986C
cmpwi r29, 0x6
blt @80089858
addi r3, r31, 0x4118
addi r0, r31, 0x40bc
li r5, 0x9
stw r3, lbl_8047A670@sda21(r0)
li r3, 0x231
li r4, 0x1
stw r5, lbl_80478960@sda21(r0)
li r5, 0x0
stw r0, lbl_8047A674@sda21(r0)
bl fn_801CA5C4
mr r29, r3
cmplwi r29, 0x2
bne @80089958
lbz r3, 0x41e1(r31)
cmplwi r3, 0x0
beq @800898CC
bl fn_801EEAD0
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @800898CC
li r0, 0x1
b @800898D0
@800898CC
li r0, 0x0
@800898D0
clrlwi r0, r0, 24
cmplwi r0, 0x1
bne @80089958
mr r3, r31
bl fn_800830A4
li r3, 0x0
bl fn_80083CFC
cmplwi r3, 0x0
beq @800898FC
lbz r4, 0x4136(r3)
b @80089900
@800898FC
li r4, 0x0
@80089900
lis r3, lbl_802EEB98@ha
li r5, 0x0
addi r3, r3, lbl_802EEB98@l
li r0, 0x10
mtctr r0
@80089914
lbz r0, 0x1(r3)
cmplw r4, r0
bne @80089934
lis r3, lbl_802EEB98@ha
slwi r0, r5, 1
addi r3, r3, lbl_802EEB98@l
lbzx r4, r3, r0
b @8008994C
@80089934
addi r3, r3, 0x2
addi r5, r5, 0x1
bdnz @80089914
lis r3, lbl_802EEB98@ha
addi r3, r3, lbl_802EEB98@l
lbz r4, 0x0(r3)
@8008994C
mr r3, r30
li r5, 0x0
bl fn_8018C1E8
@80089958
li r3, 0x1
bl fn_801C40F0
mr r3, r29
@80089964
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
