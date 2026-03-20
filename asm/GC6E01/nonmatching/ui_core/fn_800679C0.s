stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r24, 0x10(r1)
mr r27, r3
mr r24, r4
bl fn_8025DA3C
mr r30, r3
bl fn_8025DA88
li r3, 0x0
bl fn_8025D9F0
li r3, 0x1
bl fn_8025D9F0
li r3, 0x2
bl fn_8025D9F0
li r3, 0x3
bl fn_8025D9F0
cmpwi r24, 0x0
beq @80067A14
li r28, 0x1
b @80067A18
@80067A14
li r28, 0x0
@80067A18
lis r3, lbl_803A9F08@ha
addi r31, r3, lbl_803A9F08@l
addis r29, r31, 0x1
mulli r26, r28, 0x30
b @80068198
@80067A2C
bl fn_8025D9CC
cmpwi r3, 0x4
bne @80067AA8
lis r3, lbl_803A9F08@ha
mr r24, r28
addi r0, r3, lbl_803A9F08@l
add r3, r0, r28
lbz r0, 0x4(r3)
cmplwi r0, 0x0
beq @80067AA8
mr r3, r28
bl fn_8025D2B0
bl fn_8006AFE4
lwz r0, 0x4(r3)
cmpwi r0, 0x1
beq @80067A74
cmpwi r0, 0x2
bne @80067AA8
@80067A74
mr r3, r24
bl fn_8025D2B0
mr r25, r3
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80067AA8
li r0, 0x0
stb r0, -0x31a8(r29)
lwz r0, -0x31a4(r29)
cmpwi r0, 0x0
bge @80067AA8
stw r25, -0x31a4(r29)
@80067AA8
add r3, r31, r28
lbz r0, 0x4(r3)
cmplwi r0, 0x0
bne @80068190
bl fn_8025D9CC
cmpwi r3, 0x4
bne @80067C68
mr r3, r28
bl fn_8025D2B0
bl fn_8006AFE4
lwz r0, 0x4(r3)
cmpwi r0, 0x1
beq @80067AE4
cmpwi r0, 0x2
bne @80067AF8
@80067AE4
mr r3, r28
bl fn_8025D2B0
mr r4, r28
bl fn_800681B4
b @800680EC
@80067AF8
mr r3, r28
bl fn_8025D2B0
mr r24, r3
bl fn_800F7AF0
mr r25, r3
mr r3, r24
bl fn_800F7BC4
and r25, r3, r25
mr r3, r28
bl fn_8025DA18
rlwinm r0, r25, 0, 25, 25
cmplwi r0, 0x0
beq @80067B5C
mr r3, r28
bl fn_8025D560
mr r25, r3
mr r3, r28
bl fn_8025D584
cmpw r25, r3
beq @800680EC
li r3, 0x25
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
b @800680EC
@80067B5C
andi. r0, r25, 0xc0f
cmplwi r0, 0x0
beq @800680EC
mr r3, r28
li r24, -0x1
bl fn_8025D89C
clrlwi r0, r25, 31
clrlwi r3, r3, 16
cmplwi r0, 0x0
beq @80067B88
li r24, 0x0
@80067B88
rlwinm r0, r25, 0, 28, 28
cmplwi r0, 0x0
beq @80067B98
li r24, 0x1
@80067B98
rlwinm r0, r25, 0, 20, 20
cmplwi r0, 0x0
beq @80067BA8
li r24, 0x2
@80067BA8
rlwinm r0, r25, 0, 29, 29
cmplwi r0, 0x0
beq @80067BB8
li r24, 0x3
@80067BB8
rlwinm r0, r25, 0, 30, 30
cmplwi r0, 0x0
beq @80067BC8
li r24, 0x4
@80067BC8
rlwinm r0, r25, 0, 21, 21
cmplwi r0, 0x0
beq @80067BD8
li r24, 0x5
@80067BD8
cmpw r3, r24
bgt @80067BE4
li r24, -0x1
@80067BE4
cmpwi r24, 0x0
blt @800680EC
mr r3, r28
mr r4, r24
bl fn_8025D644
mr r25, r3
cmpwi r25, 0x0
blt @800680EC
li r3, 0x3c3
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
subfic r3, r25, 0x5
lis r0, 0x4330
mulli r4, r3, 0x18
lis r3, lbl_803A9F08@ha
stw r0, 0x8(r1)
slwi r0, r25, 2
addi r3, r3, lbl_803A9F08@l
lfd f2, lbl_8047BFF0@sda21(r0)
xoris r4, r4, 0x8000
lfs f0, lbl_8047BFE8@sda21(r0)
stw r4, 0xc(r1)
mulli r4, r28, 0x30
lfd f1, 0x8(r1)
add r3, r3, r4
addis r3, r3, 0x1
fsubs f1, f1, f2
add r3, r3, r0
subi r3, r3, 0x3274
stfs f1, 0x0(r3)
stfs f0, 0x18(r3)
b @800680EC
@80067C68
cmpwi r28, 0x1
bne @80067DD8
mr r3, r28
bl fn_8025D2B0
mr r3, r28
bl fn_8025DA18
lfs f2, -0x31b4(r29)
lfs f1, -0x3278(r29)
lfs f0, lbl_8047BFEC@sda21(r0)
fadds f1, f2, f1
stfs f1, -0x31b4(r29)
lfs f1, -0x31b4(r29)
fcmpo cr0, f1, f0
cror eq, gt, eq
bne @80067CCC
lfs f0, lbl_8047BFE8@sda21(r0)
lis r3, lbl_802EDB64@ha
addi r3, r3, lbl_802EDB64@l
stfs f0, -0x31b4(r29)
lwz r5, -0x31b0(r29)
slwi r4, r5, 2
addi r0, r5, 0x1
lwzx r24, r3, r4
stw r0, -0x31b0(r29)
b @80067CD0
@80067CCC
li r24, 0x0
@80067CD0
andi. r0, r24, 0xc0f
cmplwi r0, 0x0
beq @800680EC
mr r3, r28
li r25, -0x1
bl fn_8025D89C
clrlwi r0, r24, 31
clrlwi r3, r3, 16
cmplwi r0, 0x0
beq @80067CFC
li r25, 0x0
@80067CFC
rlwinm r0, r24, 0, 28, 28
cmplwi r0, 0x0
beq @80067D0C
li r25, 0x1
@80067D0C
rlwinm r0, r24, 0, 20, 20
cmplwi r0, 0x0
beq @80067D1C
li r25, 0x2
@80067D1C
rlwinm r0, r24, 0, 29, 29
cmplwi r0, 0x0
beq @80067D2C
li r25, 0x3
@80067D2C
rlwinm r0, r24, 0, 30, 30
cmplwi r0, 0x0
beq @80067D3C
li r25, 0x4
@80067D3C
rlwinm r0, r24, 0, 21, 21
cmplwi r0, 0x0
beq @80067D4C
li r25, 0x5
@80067D4C
cmpw r3, r25
mr r3, r28
bgt @80067D60
li r4, -0x1
b @80067D64
@80067D60
mr r4, r25
@80067D64
bl fn_8025D644
mr r25, r3
cmpwi r25, 0x0
blt @800680EC
li r3, 0x3c3
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
subfic r3, r25, 0x5
lis r0, 0x4330
mulli r4, r3, 0x18
lis r3, lbl_803A9F08@ha
stw r0, 0x8(r1)
slwi r0, r25, 2
addi r3, r3, lbl_803A9F08@l
lfd f2, lbl_8047BFF0@sda21(r0)
xoris r4, r4, 0x8000
lfs f0, lbl_8047BFE8@sda21(r0)
stw r4, 0xc(r1)
mulli r4, r28, 0x30
lfd f1, 0x8(r1)
add r3, r3, r4
addis r3, r3, 0x1
fsubs f1, f1, f2
add r3, r3, r0
subi r3, r3, 0x3274
stfs f1, 0x0(r3)
stfs f0, 0x18(r3)
b @800680EC
@80067DD8
mr r3, r28
bl fn_8025D9F0
clrlwi r0, r3, 16
cmpwi r0, 0x0
beq @80067DFC
blt @80067F80
cmpwi r0, 0x3
bge @80067F80
b @80067F6C
@80067DFC
mr r3, r28
bl fn_8025D2B0
mr r24, r3
bl fn_800F7AF0
mr r25, r3
mr r3, r24
bl fn_800F7BC4
and r24, r3, r25
mr r3, r28
bl fn_8025DA18
rlwinm r0, r24, 0, 25, 25
cmplwi r0, 0x0
beq @80067E60
mr r3, r28
bl fn_8025D560
mr r25, r3
mr r3, r28
bl fn_8025D584
cmpw r25, r3
beq @800680EC
li r3, 0x25
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
b @800680EC
@80067E60
andi. r0, r24, 0xc0f
cmplwi r0, 0x0
beq @800680EC
mr r3, r28
li r25, -0x1
bl fn_8025D89C
clrlwi r0, r24, 31
clrlwi r3, r3, 16
cmplwi r0, 0x0
beq @80067E8C
li r25, 0x0
@80067E8C
rlwinm r0, r24, 0, 28, 28
cmplwi r0, 0x0
beq @80067E9C
li r25, 0x1
@80067E9C
rlwinm r0, r24, 0, 20, 20
cmplwi r0, 0x0
beq @80067EAC
li r25, 0x2
@80067EAC
rlwinm r0, r24, 0, 29, 29
cmplwi r0, 0x0
beq @80067EBC
li r25, 0x3
@80067EBC
rlwinm r0, r24, 0, 30, 30
cmplwi r0, 0x0
beq @80067ECC
li r25, 0x4
@80067ECC
rlwinm r0, r24, 0, 21, 21
cmplwi r0, 0x0
beq @80067EDC
li r25, 0x5
@80067EDC
cmpw r3, r25
bgt @80067EE8
li r25, -0x1
@80067EE8
cmpwi r25, 0x0
blt @800680EC
mr r3, r28
mr r4, r25
bl fn_8025D644
mr r25, r3
cmpwi r25, 0x0
blt @800680EC
li r3, 0x3c3
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
subfic r3, r25, 0x5
lis r0, 0x4330
mulli r4, r3, 0x18
lis r3, lbl_803A9F08@ha
stw r0, 0x8(r1)
slwi r0, r25, 2
addi r3, r3, lbl_803A9F08@l
lfd f2, lbl_8047BFF0@sda21(r0)
xoris r4, r4, 0x8000
lfs f0, lbl_8047BFE8@sda21(r0)
stw r4, 0xc(r1)
mulli r4, r28, 0x30
lfd f1, 0x8(r1)
add r3, r3, r4
addis r3, r3, 0x1
fsubs f1, f1, f2
add r3, r3, r0
subi r3, r3, 0x3274
stfs f1, 0x0(r3)
stfs f0, 0x18(r3)
b @800680EC
@80067F6C
mr r3, r28
bl fn_8025D2B0
mr r4, r28
bl fn_800681B4
b @800680EC
@80067F80
mr r3, r28
bl fn_8025D2B0
mr r24, r3
bl fn_800F7AF0
mr r25, r3
mr r3, r24
bl fn_800F7BC4
and r24, r3, r25
mr r3, r28
bl fn_8025DA18
rlwinm r0, r24, 0, 25, 25
cmplwi r0, 0x0
beq @80067FE4
mr r3, r28
bl fn_8025D560
mr r25, r3
mr r3, r28
bl fn_8025D584
cmpw r25, r3
beq @800680EC
li r3, 0x25
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
b @800680EC
@80067FE4
andi. r0, r24, 0xc0f
cmplwi r0, 0x0
beq @800680EC
mr r3, r28
li r25, -0x1
bl fn_8025D89C
clrlwi r0, r24, 31
clrlwi r3, r3, 16
cmplwi r0, 0x0
beq @80068010
li r25, 0x0
@80068010
rlwinm r0, r24, 0, 28, 28
cmplwi r0, 0x0
beq @80068020
li r25, 0x1
@80068020
rlwinm r0, r24, 0, 20, 20
cmplwi r0, 0x0
beq @80068030
li r25, 0x2
@80068030
rlwinm r0, r24, 0, 29, 29
cmplwi r0, 0x0
beq @80068040
li r25, 0x3
@80068040
rlwinm r0, r24, 0, 30, 30
cmplwi r0, 0x0
beq @80068050
li r25, 0x4
@80068050
rlwinm r0, r24, 0, 21, 21
cmplwi r0, 0x0
beq @80068060
li r25, 0x5
@80068060
cmpw r3, r25
bgt @8006806C
li r25, -0x1
@8006806C
cmpwi r25, 0x0
blt @800680EC
mr r3, r28
mr r4, r25
bl fn_8025D644
mr r25, r3
cmpwi r25, 0x0
blt @800680EC
li r3, 0x3c3
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
subfic r3, r25, 0x5
lis r0, 0x4330
mulli r4, r3, 0x18
lis r3, lbl_803A9F08@ha
stw r0, 0x8(r1)
slwi r0, r25, 2
addi r3, r3, lbl_803A9F08@l
lfd f2, lbl_8047BFF0@sda21(r0)
xoris r4, r4, 0x8000
lfs f0, lbl_8047BFE8@sda21(r0)
stw r4, 0xc(r1)
mulli r4, r28, 0x30
lfd f1, 0x8(r1)
add r3, r3, r4
addis r3, r3, 0x1
fsubs f1, f1, f2
add r3, r3, r0
subi r3, r3, 0x3274
stfs f1, 0x0(r3)
stfs f0, 0x18(r3)
@800680EC
mr r3, r28
bl fn_8025D2B0
cmpwi r3, 0x1
bne @80068190
mr r3, r28
bl fn_8025D9F0
clrlwi r0, r3, 16
cmplwi r0, 0x0
bne @80068190
mr r3, r28
bl fn_8025D560
mr r25, r3
bl fn_8006B1D4
clrlwi r24, r3, 16
mr r3, r28
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r24
bge @8006813C
b @80068140
@8006813C
mr r0, r24
@80068140
clrlwi r0, r0, 16
cmpw r25, r0
bne @80068190
subi r0, r25, 0x1
cmpwi r0, 0x0
bge @8006815C
li r0, 0x0
@8006815C
lis r3, lbl_803A9F08@ha
slwi r0, r0, 2
addi r3, r3, lbl_803A9F08@l
lfs f1, lbl_8047BFE8@sda21(r0)
add r3, r3, r26
add r3, r3, r0
addis r3, r3, 0x1
lfs f0, -0x3274(r3)
fcmpu cr0, f1, f0
bne @80068190
li r0, 0x1
stb r0, 0x95(r27)
stb r0, 0x98(r27)
@80068190
addi r28, r28, 0x1
addi r26, r26, 0x30
@80068198
cmpw r28, r30
blt @80067A2C
lmw r24, 0x10(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
