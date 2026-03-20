stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r25, 0x14(r1)
mr r25, r3
mr r26, r4
mr r27, r5
mr r28, r6
mr r29, r7
lis r3, lbl_803A9A60@ha
addi r5, r3, lbl_803A9A60@l
lwz r0, 0x4(r5)
cmpwi r0, 0x1
beq @80061C18
bge @80061D20
cmpwi r0, 0x0
bge @80061C04
b @80061D20
@80061C04
lbz r0, 0x4(r26)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r26)
b @80061D20
@80061C18
mulli r0, r27, 0xb4
lha r4, 0x6(r26)
lis r3, lbl_802EF0A8@ha
slwi r31, r28, 2
add r5, r5, r0
addi r0, r3, lbl_802EF0A8@l
addi r30, r5, 0x58
add r3, r30, r31
lfs f0, 0x3c(r3)
mulli r3, r4, 0x1c
fctiwz f0, f0
add r3, r0, r3
lha r3, 0x2(r3)
stfd f0, 0x8(r1)
lwz r0, 0xc(r1)
add r0, r3, r0
extsh r0, r0
sth r0, 0x50(r26)
lha r5, 0x84(r25)
lha r3, 0x50(r26)
lha r4, 0x86(r25)
lha r0, 0x52(r26)
add r3, r5, r3
extsh r3, r3
add r0, r4, r0
extsh r4, r0
bl fn_800FE6D0
bl fn_800FE4D4
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
lwz r0, 0x38(r3)
cmpwi r0, 0x5
blt @80061D10
mr r3, r25
mr r4, r26
mr r5, r27
mr r6, r28
mr r7, r29
bl fn_80061D34
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80061CFC
add r3, r30, r31
lfs f1, lbl_8047BF60@sda21(r0)
lfs f0, 0x84(r3)
fcmpu cr0, f1, f0
bne @80061CE8
lbz r0, 0x4(r26)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r26)
b @80061D20
@80061CE8
lbz r0, 0x4(r26)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r26)
b @80061D20
@80061CFC
lbz r0, 0x4(r26)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r26)
b @80061D20
@80061D10
lbz r0, 0x4(r26)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r26)
@80061D20
lmw r25, 0x14(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
