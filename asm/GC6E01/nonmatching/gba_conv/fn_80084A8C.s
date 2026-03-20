stwu r1, -0xcd0(r1)
mflr r0
stw r0, 0xcd4(r1)
stfd f31, 0xcc0(r1)
psq_st f31, -0x338(r1), 0, 0
stfd f30, 0xcb0(r1)
psq_st f30, -0x348(r1), 0, 0
stfd f29, 0xca0(r1)
psq_st f29, -0x358(r1), 0, 0
stfd f28, 0xc90(r1)
psq_st f28, -0x368(r1), 0, 0
stfd f27, 0xc80(r1)
psq_st f27, -0x378(r1), 0, 0
stmw r15, 0xc3c(r1)
stw r3, 0x8(r1)
stw r4, 0xc(r1)
stw r5, 0x10(r1)
stw r6, 0x14(r1)
lis r3, lbl_8026F2E8@ha
li r15, 0x0
addi r0, r3, lbl_8026F2E8@l
stw r0, 0xc34(r1)
li r3, 0x1
bl fn_80093698
b @80084AF4
@80084AF0
bl fn_800F0308
@80084AF4
lwz r3, 0xc34(r1)
li r5, 0x0
addi r4, r3, 0x190
li r3, 0x1
bl fn_800932F0
cmpwi r3, 0x0
beq @80084AF0
li r3, 0xe4
li r4, 0x0
bl fn_8010264C
li r3, 0xe4
bl fn_80104704
mr r16, r3
cmplwi r16, 0x0
bne @80084B44
lwz r3, 0xc34(r1)
li r4, 0x1f4
li r5, lbl_8047C1A0@sda21
addi r3, r3, 0x184
bl fn_80196E10
@80084B44
cmplwi r16, 0x0
bne @80084B58
li r3, 0xa6
bl fn_80104704
mr r16, r3
@80084B58
mr r3, r16
bl fn_801040A0
lwz r0, 0x8(r1)
li r5, 0x0
lwz r16, 0x0(r3)
li r30, lbl_80478954@sda21
slwi r4, r0, 4
lwz r0, 0xc(r1)
lwz r3, 0xc34(r1)
stw r0, 0x24(r16)
lwz r0, 0x8(r1)
addi r29, r3, 0x30
add r29, r29, r4
stw r0, 0x2c(r16)
lbz r0, lbl_80478954@sda21(r0)
stb r0, 0x21(r16)
li r0, 0x5
lbz r3, 0x0(r30)
cmpwi r5, 0x0
extsb r3, r3
slwi r5, r3, 2
lwzx r3, r29, r5
stwx r3, r16, r5
bge @80084BC8
lwzx r3, r16, r5
cmpwi r3, 0x1
bne @80084BC8
stwx r0, r16, r5
@80084BC8
addi r4, r30, 0x1
li r5, 0x1
lbz r3, 0x0(r4)
cmpwi r5, 0x0
extsb r3, r3
slwi r5, r3, 2
lwzx r3, r29, r5
stwx r3, r16, r5
bge @80084BFC
lwzx r3, r16, r5
cmpwi r3, 0x1
bne @80084BFC
stwx r0, r16, r5
@80084BFC
addi r4, r4, 0x1
li r5, 0x2
lbz r3, 0x0(r4)
cmpwi r5, 0x0
extsb r3, r3
slwi r5, r3, 2
lwzx r3, r29, r5
stwx r3, r16, r5
bge @80084C30
lwzx r3, r16, r5
cmpwi r3, 0x1
bne @80084C30
stwx r0, r16, r5
@80084C30
addi r4, r4, 0x1
li r5, 0x3
lbz r3, 0x0(r4)
cmpwi r5, 0x0
extsb r3, r3
slwi r5, r3, 2
lwzx r3, r29, r5
stwx r3, r16, r5
bge @80084C64
lwzx r3, r16, r5
cmpwi r3, 0x1
bne @80084C64
stwx r0, r16, r5
@80084C64
lwz r0, 0xc(r1)
mr r24, r16
rlwinm r0, r0, 0, 27, 27
cmplwi r0, 0x0
beq @80084D98
lwz r0, 0x8(r1)
cmpwi r0, 0x0
beq @80084C8C
cmpwi r0, 0x2
bne @80084D98
@80084C8C
li r3, 0x0
li r4, 0x2
bl fn_80129280
lwz r0, 0xc(r1)
li r4, 0x0
stb r4, 0x21(r16)
li r4, 0x8
rlwinm r0, r0, 0, 30, 30
mr r18, r3
cmplwi r0, 0x0
stw r4, 0x0(r16)
beq @80084D6C
li r3, 0x2f
li r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x3d88
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lfs f27, lbl_8047C1A8@sda21(r0)
lfd f31, lbl_8047C1B0@sda21(r0)
lis r17, 0x4330
lfd f29, lbl_8047C1B8@sda21(r0)
lfs f28, lbl_8047C1AC@sda21(r0)
b @80084D2C
@80084CF4
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r17, 0xc10(r1)
stw r0, 0xc14(r1)
lfd f0, 0xc10(r1)
fsubs f30, f0, f31
bl fn_800D3088
stw r3, 0xc1c(r1)
stw r17, 0xc18(r1)
lfd f0, 0xc18(r1)
fsubs f0, f0, f29
fdivs f0, f0, f30
fadds f27, f27, f0
@80084D2C
fcmpo cr0, f27, f28
blt @80084CF4
mr r3, r18
bl fn_800776E4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80084D6C
li r3, 0xe4
li r4, 0x0
li r5, 0x1
bl fn_80102568
mr r3, r18
li r4, 0x0
bl fn_8005CF2C
li r3, 0x0
b @80087AAC
@80084D6C
lwz r0, 0x10(r1)
cmplwi r0, 0x0
beq @80084D90
mr r3, r0
lwz r3, 0x0(r3)
cmplwi r3, 0x0
beq @80084D90
mr r4, r18
bl fn_8012AC64
@80084D90
li r0, 0xa
stw r0, 0x0(r16)
@80084D98
lwz r0, 0xc(r1)
li r25, 0x0
stw r30, 0xc30(r1)
rlwinm r0, r0, 0, 25, 25
stw r0, 0xc2c(r1)
lwz r0, 0xc(r1)
clrlwi r0, r0, 31
stw r0, 0xc28(r1)
lwz r0, 0xc(r1)
rlwinm r0, r0, 0, 30, 30
stw r0, 0xc24(r1)
lwz r0, 0xc(r1)
rlwinm r0, r0, 0, 28, 28
stw r0, 0xc20(r1)
lwz r0, 0xc(r1)
rlwinm r31, r0, 0, 26, 26
@80084DD8
lwz r3, 0xc30(r1)
lwz r0, 0x10(r1)
lbz r23, 0x0(r3)
cmplwi r0, 0x0
stb r23, 0x21(r24)
beq @80084E10
extsb r0, r23
lwz r3, 0x10(r1)
slwi r0, r0, 2
lwzx r0, r3, r0
cmplwi r0, 0x0
beq @80084E10
mr r22, r0
b @80084E14
@80084E10
addi r22, r1, 0xf4
@80084E14
lwz r0, 0x14(r1)
cmplwi r0, 0x0
beq @80084E34
extsb r0, r23
cmpwi r0, 0x1
bne @80084E34
lwz r21, 0x14(r1)
b @80084E38
@80084E34
addi r21, r1, 0x1c
@80084E38
extsb r28, r23
addi r26, r28, 0x1
slwi r27, r28, 2
@80084E44
bl fn_80103CB0
li r4, lbl_80478950@sda21
lbzx r0, r4, r28
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
lwz r0, 0x28(r24)
cmplwi r0, 0x4
beq @8008507C
li r0, 0x2
mr r4, r26
stwx r0, r27, r24
li r3, 0x2f
bl fn_80132A38
li r3, 0x7
li r4, 0x3c42
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
mr r3, r24
li r4, 0x6
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8008507C
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@80084EB4
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@80084EC4
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @80084ED8
cmpwi r0, 0x4
bne @80084F10
@80084ED8
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80084F10
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80084F10
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80084F20
li r16, 0x1
@80084F20
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @80084EC4
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80084F50
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @80084EB4
@80084F50
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80084F9C
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80084F9C
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80084F9C
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80084F9C
li r15, 0x4
@80084F9C
cmpwi r15, 0x3
ble @80084FBC
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80084FBC
li r3, 0x0
b @80085010
@80084FBC
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80084FF0
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085004
@80084FF0
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085004
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80085010
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80085030
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80085030
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80085054
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085068
@80085054
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085068
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@8008507C
mr r4, r26
li r3, 0x2f
bl fn_80132A38
li r3, 0x7
li r4, 0x3c43
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
clrlwi r0, r15, 24
li r3, 0x3
cmplwi r0, 0x0
stwx r3, r27, r24
bne @8008531C
li r0, 0x0
stw r0, 0x28(r24)
b @80085114
@800850BC
li r3, 0x10c
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800850D8
bl fn_800F0308
b @80085114
@800850D8
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 26, 26
cmpwi r0, 0x0
beq @800850FC
li r0, 0x2
li r3, 0xe
stw r0, 0x28(r24)
b @8008513C
@800850FC
lwz r0, 0x28(r24)
cmplwi r0, 0x8
bne @80085110
li r3, 0xe
b @8008513C
@80085110
bl fn_800F0308
@80085114
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_800934E4
cmpwi r3, 0x0
beq @800850BC
li r0, 0x0
stw r0, 0x28(r24)
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093610
@8008513C
cmpwi r3, 0xe
bne @80085318
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@80085150
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@80085160
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @80085174
cmpwi r0, 0x4
bne @800851AC
@80085174
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800851AC
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@800851AC
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @800851BC
li r16, 0x1
@800851BC
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @80085160
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @800851EC
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @80085150
@800851EC
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80085238
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80085238
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80085238
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80085238
li r15, 0x4
@80085238
cmpwi r15, 0x3
ble @80085258
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80085258
li r3, 0x0
b @800852AC
@80085258
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @8008528C
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @800852A0
@8008528C
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@800852A0
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@800852AC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800852CC
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@800852CC
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @800852F0
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085304
@800852F0
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085304
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@80085318
li r15, 0x1
@8008531C
mr r3, r28
li r4, 0x0
bl fn_80093160
li r0, 0x0
stw r0, 0x28(r24)
b @8008538C
@80085334
li r3, 0x10c
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80085350
bl fn_800F0308
b @8008538C
@80085350
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 26, 26
cmpwi r0, 0x0
beq @80085374
li r0, 0x2
li r3, 0xe
stw r0, 0x28(r24)
b @800853B4
@80085374
lwz r0, 0x28(r24)
cmplwi r0, 0x8
bne @80085388
li r3, 0xe
b @800853B4
@80085388
bl fn_800F0308
@8008538C
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_800934E4
cmpwi r3, 0x0
beq @80085334
li r0, 0x0
stw r0, 0x28(r24)
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093610
@800853B4
cmpwi r3, 0xe
beq @800853E0
bge @800853CC
cmpwi r3, 0x2
beq @800857E0
b @800855B4
@800853CC
lis r4, 0x2
addi r0, r4, 0x2
cmpw r3, r0
beq @800855B4
b @800855B4
@800853E0
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@800853EC
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@800853FC
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @80085410
cmpwi r0, 0x4
bne @80085448
@80085410
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80085448
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80085448
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80085458
li r16, 0x1
@80085458
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @800853FC
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80085488
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @800853EC
@80085488
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800854D4
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800854D4
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800854D4
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800854D4
li r15, 0x4
@800854D4
cmpwi r15, 0x3
ble @800854F4
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800854F4
li r3, 0x0
b @80085548
@800854F4
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80085528
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8008553C
@80085528
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@8008553C
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80085548
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80085568
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80085568
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @8008558C
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @800855A0
@8008558C
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@800855A0
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@800855B4
li r20, 0x0
@800855B8
li r19, 0x0
mr r17, r24
mr r18, r19
li r16, lbl_80478950@sda21
@800855C8
lwz r0, 0x0(r17)
cmpwi r0, 0x5
beq @800855DC
cmpwi r0, 0x4
bne @80085614
@800855DC
addi r3, r18, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80085614
bl fn_80103CB0
lbz r0, 0x0(r16)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r17)
stw r0, 0x28(r24)
@80085614
lwz r0, 0x0(r17)
cmpwi r0, 0x7
bne @80085624
li r19, 0x1
@80085624
addi r17, r17, 0x4
addi r16, r16, 0x1
addi r18, r18, 0x1
cmpwi r18, 0x3
ble @800855C8
clrlwi r0, r19, 24
cmplwi r0, 0x0
bne @80085654
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @800855B8
@80085654
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800856A0
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800856A0
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800856A0
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800856A0
li r16, 0x4
@800856A0
cmpwi r16, 0x3
ble @800856C0
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800856C0
li r3, 0x0
b @80085714
@800856C0
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @800856F4
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085708
@800856F4
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085708
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80085714
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80085740
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @80085738
li r0, 0x1
b @800857CC
@80085738
li r0, 0x0
b @800857CC
@80085740
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x3c47
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @8008579C
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @800857BC
@8008579C
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800857BC
li r0, 0x1
b @800857CC
@800857BC
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@800857CC
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x0
b @80087AAC
@800857E0
li r0, 0x4
li r16, 0x0
stwx r0, r27, r24
@800857EC
mr r3, r26
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008581C
bl fn_80103CB0
li r4, lbl_80478950@sda21
lbzx r0, r4, r28
or r0, r0, r3
clrlwi r3, r0, 24
bl fn_80103CC0
b @8008582C
@8008581C
bl fn_800F0308
addi r16, r16, 0x1
cmpwi r16, 0x12c
blt @800857EC
@8008582C
lwz r0, 0xc2c(r1)
cmplwi r0, 0x0
bne @800858B0
mr r4, r26
li r3, 0x2f
bl fn_80132A38
li r3, 0x7
li r4, 0x3c4d
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lfs f27, lbl_8047C1A8@sda21(r0)
lfd f28, lbl_8047C1B0@sda21(r0)
lis r16, 0x4330
lfd f30, lbl_8047C1B8@sda21(r0)
lfs f31, lbl_8047C1AC@sda21(r0)
b @800858A8
@80085870
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r16, 0xc18(r1)
stw r0, 0xc1c(r1)
lfd f0, 0xc18(r1)
fsubs f29, f0, f28
bl fn_800D3088
stw r3, 0xc14(r1)
stw r16, 0xc10(r1)
lfd f0, 0xc10(r1)
fsubs f0, f0, f30
fdivs f0, f0, f29
fadds f27, f27, f0
@800858A8
fcmpo cr0, f27, f31
blt @80085870
@800858B0
mr r3, r22
bl fn_8012A248
li r0, 0x0
mr r3, r28
stw r0, 0x18(r1)
mr r4, r22
addi r5, r1, 0x18
bl fn_80092FC8
li r0, 0x0
stw r0, 0x28(r24)
b @80085934
@800858DC
li r3, 0x10c
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800858F8
bl fn_800F0308
b @80085934
@800858F8
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 26, 26
cmpwi r0, 0x0
beq @8008591C
li r0, 0x2
li r16, 0xe
stw r0, 0x28(r24)
b @80085960
@8008591C
lwz r0, 0x28(r24)
cmplwi r0, 0x8
bne @80085930
li r16, 0xe
b @80085960
@80085930
bl fn_800F0308
@80085934
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_800934E4
cmpwi r3, 0x0
beq @800858DC
li r0, 0x0
stw r0, 0x28(r24)
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093610
mr r16, r3
@80085960
cmpwi r16, 0xe
beq @8008596C
b @80085B40
@8008596C
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@80085978
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@80085988
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @8008599C
cmpwi r0, 0x4
bne @800859D4
@8008599C
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800859D4
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@800859D4
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @800859E4
li r16, 0x1
@800859E4
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @80085988
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80085A14
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @80085978
@80085A14
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80085A60
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80085A60
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80085A60
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80085A60
li r15, 0x4
@80085A60
cmpwi r15, 0x3
ble @80085A80
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80085A80
li r3, 0x0
b @80085AD4
@80085A80
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80085AB4
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085AC8
@80085AB4
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085AC8
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80085AD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80085AF4
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80085AF4
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80085B18
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085B2C
@80085B18
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085B2C
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@80085B40
lwz r3, 0x18(r1)
extrwi r0, r3, 2, 22
cmplwi r0, 0x0
beq @80085D7C
li r20, 0x0
@80085B54
li r18, 0x0
mr r16, r24
mr r19, r18
li r17, lbl_80478950@sda21
@80085B64
lwz r0, 0x0(r16)
cmpwi r0, 0x5
beq @80085B78
cmpwi r0, 0x4
bne @80085BB0
@80085B78
addi r3, r19, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80085BB0
bl fn_80103CB0
lbz r0, 0x0(r17)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r16)
stw r0, 0x28(r24)
@80085BB0
lwz r0, 0x0(r16)
cmpwi r0, 0x7
bne @80085BC0
li r18, 0x1
@80085BC0
addi r16, r16, 0x4
addi r17, r17, 0x1
addi r19, r19, 0x1
cmpwi r19, 0x3
ble @80085B64
clrlwi r0, r18, 24
cmplwi r0, 0x0
bne @80085BF0
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @80085B54
@80085BF0
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80085C3C
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80085C3C
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80085C3C
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80085C3C
li r16, 0x4
@80085C3C
cmpwi r16, 0x3
ble @80085C5C
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80085C5C
li r3, 0x0
b @80085CB0
@80085C5C
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @80085C90
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085CA4
@80085C90
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085CA4
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80085CB0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80085CDC
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @80085CD4
li r0, 0x1
b @80085D68
@80085CD4
li r0, 0x0
b @80085D68
@80085CDC
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x3c49
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80085D38
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @80085D58
@80085D38
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80085D58
li r0, 0x1
b @80085D68
@80085D58
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@80085D68
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x0
b @80087AAC
@80085D7C
extrwi r0, r3, 4, 24
cmplwi r0, 0x7
bgt @80085DD0
lis r3, jumptable_802EEB78@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EEB78@l
lwzx r0, r3, r0
mtctr r0
bctr
li r17, 0x1
b @80085DD8
li r17, 0x2
b @80085DD8
li r17, 0x4
b @80085DD8
li r17, 0x5
b @80085DD8
li r17, 0x3
b @80085DD8
li r17, 0x6
b @80085DD8
@80085DD0
li r0, 0x0
b @80085E1C
@80085DD8
bl fn_80128E24
cmplwi r3, 0x0
beq @80085E18
bl fn_80128E04
cmplwi r3, 0x0
beq @80085E18
bl fn_80135B0C
cmplwi r3, 0x0
beq @80085E18
bl fn_80135A70
clrlwi r3, r3, 24
clrlwi r0, r17, 24
cmplw r3, r0
bne @80085E18
li r0, 0x1
b @80085E1C
@80085E18
li r0, 0x0
@80085E1C
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80086054
li r20, 0x0
@80085E2C
li r18, 0x0
mr r16, r24
mr r19, r18
li r17, lbl_80478950@sda21
@80085E3C
lwz r0, 0x0(r16)
cmpwi r0, 0x5
beq @80085E50
cmpwi r0, 0x4
bne @80085E88
@80085E50
addi r3, r19, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80085E88
bl fn_80103CB0
lbz r0, 0x0(r17)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r16)
stw r0, 0x28(r24)
@80085E88
lwz r0, 0x0(r16)
cmpwi r0, 0x7
bne @80085E98
li r18, 0x1
@80085E98
addi r16, r16, 0x4
addi r17, r17, 0x1
addi r19, r19, 0x1
cmpwi r19, 0x3
ble @80085E3C
clrlwi r0, r18, 24
cmplwi r0, 0x0
bne @80085EC8
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @80085E2C
@80085EC8
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80085F14
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80085F14
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80085F14
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80085F14
li r16, 0x4
@80085F14
cmpwi r16, 0x3
ble @80085F34
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80085F34
li r3, 0x0
b @80085F88
@80085F34
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @80085F68
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80085F7C
@80085F68
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80085F7C
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80085F88
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80085FB4
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @80085FAC
li r0, 0x1
b @80086040
@80085FAC
li r0, 0x0
b @80086040
@80085FB4
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x44f0
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086010
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @80086030
@80086010
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086030
li r0, 0x1
b @80086040
@80086030
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@80086040
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x0
b @80087AAC
@80086054
cmpwi r16, 0x4
beq @80086060
b @80086070
@80086060
lwz r3, 0x18(r1)
rlwinm r0, r3, 0, 30, 30
cmplwi r0, 0x0
bne @8008629C
@80086070
li r20, 0x0
@80086074
li r18, 0x0
mr r16, r24
mr r19, r18
li r17, lbl_80478950@sda21
@80086084
lwz r0, 0x0(r16)
cmpwi r0, 0x5
beq @80086098
cmpwi r0, 0x4
bne @800860D0
@80086098
addi r3, r19, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800860D0
bl fn_80103CB0
lbz r0, 0x0(r17)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r16)
stw r0, 0x28(r24)
@800860D0
lwz r0, 0x0(r16)
cmpwi r0, 0x7
bne @800860E0
li r18, 0x1
@800860E0
addi r16, r16, 0x4
addi r17, r17, 0x1
addi r19, r19, 0x1
cmpwi r19, 0x3
ble @80086084
clrlwi r0, r18, 24
cmplwi r0, 0x0
bne @80086110
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @80086074
@80086110
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @8008615C
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @8008615C
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @8008615C
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @8008615C
li r16, 0x4
@8008615C
cmpwi r16, 0x3
ble @8008617C
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @8008617C
li r3, 0x0
b @800861D0
@8008617C
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @800861B0
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @800861C4
@800861B0
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@800861C4
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@800861D0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800861FC
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @800861F4
li r0, 0x1
b @80086288
@800861F4
li r0, 0x0
b @80086288
@800861FC
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x3c49
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086258
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @80086278
@80086258
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086278
li r0, 0x1
b @80086288
@80086278
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@80086288
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x0
b @80087AAC
@8008629C
lwz r0, 0xc28(r1)
cmplwi r0, 0x0
beq @80086514
rlwinm r0, r3, 0, 29, 29
li r4, 0x1
cmplwi r0, 0x0
beq @800862CC
rlwinm r0, r3, 0, 28, 28
cmplwi r0, 0x0
bne @800862DC
li r4, 0x0
b @800862DC
@800862CC
clrlwi r0, r3, 31
cmplwi r0, 0x0
bne @800862DC
li r4, 0x0
@800862DC
clrlwi r0, r4, 24
cmplwi r0, 0x0
bne @80086514
li r20, 0x0
@800862EC
li r18, 0x0
mr r16, r24
mr r19, r18
li r17, lbl_80478950@sda21
@800862FC
lwz r0, 0x0(r16)
cmpwi r0, 0x5
beq @80086310
cmpwi r0, 0x4
bne @80086348
@80086310
addi r3, r19, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80086348
bl fn_80103CB0
lbz r0, 0x0(r17)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r16)
stw r0, 0x28(r24)
@80086348
lwz r0, 0x0(r16)
cmpwi r0, 0x7
bne @80086358
li r18, 0x1
@80086358
addi r16, r16, 0x4
addi r17, r17, 0x1
addi r19, r19, 0x1
cmpwi r19, 0x3
ble @800862FC
clrlwi r0, r18, 24
cmplwi r0, 0x0
bne @80086388
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @800862EC
@80086388
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800863D4
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800863D4
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800863D4
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800863D4
li r16, 0x4
@800863D4
cmpwi r16, 0x3
ble @800863F4
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800863F4
li r3, 0x0
b @80086448
@800863F4
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @80086428
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8008643C
@80086428
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@8008643C
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80086448
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086474
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @8008646C
li r0, 0x1
b @80086500
@8008646C
li r0, 0x0
b @80086500
@80086474
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x44c3
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800864D0
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @800864F0
@800864D0
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800864F0
li r0, 0x1
b @80086500
@800864F0
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@80086500
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x0
b @80087AAC
@80086514
lwz r0, 0xc24(r1)
cmplwi r0, 0x0
beq @800868EC
clrlwi r0, r3, 31
cmplwi r0, 0x0
bne @80086758
li r20, 0x0
@80086530
li r18, 0x0
mr r16, r24
mr r19, r18
li r17, lbl_80478950@sda21
@80086540
lwz r0, 0x0(r16)
cmpwi r0, 0x5
beq @80086554
cmpwi r0, 0x4
bne @8008658C
@80086554
addi r3, r19, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8008658C
bl fn_80103CB0
lbz r0, 0x0(r17)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r16)
stw r0, 0x28(r24)
@8008658C
lwz r0, 0x0(r16)
cmpwi r0, 0x7
bne @8008659C
li r18, 0x1
@8008659C
addi r16, r16, 0x4
addi r17, r17, 0x1
addi r19, r19, 0x1
cmpwi r19, 0x3
ble @80086540
clrlwi r0, r18, 24
cmplwi r0, 0x0
bne @800865CC
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @80086530
@800865CC
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80086618
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80086618
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80086618
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80086618
li r16, 0x4
@80086618
cmpwi r16, 0x3
ble @80086638
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086638
li r3, 0x0
b @8008668C
@80086638
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @8008666C
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80086680
@8008666C
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80086680
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@8008668C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800866B8
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @800866B0
li r0, 0x1
b @80086744
@800866B0
li r0, 0x0
b @80086744
@800866B8
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x44c3
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086714
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @80086734
@80086714
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086734
li r0, 0x1
b @80086744
@80086734
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@80086744
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x0
b @80087AAC
@80086758
mr r3, r22
bl fn_800776E4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800868EC
li r3, 0xe4
li r4, 0x0
li r5, 0x1
bl fn_80102568
mr r3, r22
li r4, 0x1
bl fn_8005CF2C
lwz r0, 0xc20(r1)
cmplwi r0, 0x0
bne @8008679C
li r3, 0x0
b @80087AAC
@8008679C
li r3, 0xe4
li r4, 0x0
bl fn_8010264C
li r3, 0xe4
bl fn_80104704
mr r16, r3
cmplwi r16, 0x0
bne @800867D0
lwz r3, 0xc34(r1)
li r4, 0x1f4
li r5, lbl_8047C1A0@sda21
addi r3, r3, 0x184
bl fn_80196E10
@800867D0
cmplwi r16, 0x0
bne @800867E4
li r3, 0xa6
bl fn_80104704
mr r16, r3
@800867E4
mr r3, r16
bl fn_801040A0
lwz r5, 0x0(r3)
li r4, 0x0
lwz r0, 0xc(r1)
lwz r3, 0xc30(r1)
stw r0, 0x24(r5)
lwz r0, 0x8(r1)
stw r0, 0x2c(r5)
lbz r0, 0x0(r3)
stb r0, 0x21(r5)
li r0, 0x5
lbz r3, 0x0(r30)
cmpw r4, r25
extsb r3, r3
slwi r4, r3, 2
lwzx r3, r29, r4
stwx r3, r5, r4
bge @80086840
lwzx r3, r5, r4
cmpwi r3, 0x1
bne @80086840
stwx r0, r5, r4
@80086840
addi r6, r30, 0x1
li r4, 0x1
lbz r3, 0x0(r6)
cmpw r4, r25
extsb r3, r3
slwi r4, r3, 2
lwzx r3, r29, r4
stwx r3, r5, r4
bge @80086874
lwzx r3, r5, r4
cmpwi r3, 0x1
bne @80086874
stwx r0, r5, r4
@80086874
addi r6, r6, 0x1
li r4, 0x2
lbz r3, 0x0(r6)
cmpw r4, r25
extsb r3, r3
slwi r4, r3, 2
lwzx r3, r29, r4
stwx r3, r5, r4
bge @800868A8
lwzx r3, r5, r4
cmpwi r3, 0x1
bne @800868A8
stwx r0, r5, r4
@800868A8
addi r6, r6, 0x1
li r4, 0x3
lbz r3, 0x0(r6)
cmpw r4, r25
extsb r3, r3
slwi r4, r3, 2
lwzx r3, r29, r4
stwx r3, r5, r4
bge @800868DC
lwzx r3, r5, r4
cmpwi r3, 0x1
bne @800868DC
stwx r0, r5, r4
@800868DC
li r0, 0x6
mr r24, r5
stwx r0, r27, r5
b @80084E44
@800868EC
lwz r0, 0x14(r1)
cmplwi r0, 0x0
beq @80086904
extsb r0, r23
cmpwi r0, 0x1
beq @8008690C
@80086904
cmplwi r31, 0x0
beq @80086DBC
@8008690C
mr r3, r28
mr r4, r21
bl fn_80092E38
li r0, 0x0
stw r0, 0x28(r24)
b @8008697C
@80086924
li r3, 0x10c
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086940
bl fn_800F0308
b @8008697C
@80086940
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 26, 26
cmpwi r0, 0x0
beq @80086964
li r0, 0x2
li r3, 0xe
stw r0, 0x28(r24)
b @800869A4
@80086964
lwz r0, 0x28(r24)
cmplwi r0, 0x8
bne @80086978
li r3, 0xe
b @800869A4
@80086978
bl fn_800F0308
@8008697C
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_800934E4
cmpwi r3, 0x0
beq @80086924
li r0, 0x0
stw r0, 0x28(r24)
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093610
@800869A4
cmpwi r3, 0xe
beq @800869BC
bge @80086B90
cmpwi r3, 0xb
beq @80086DBC
b @80086B90
@800869BC
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@800869C8
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@800869D8
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @800869EC
cmpwi r0, 0x4
bne @80086A24
@800869EC
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80086A24
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80086A24
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80086A34
li r16, 0x1
@80086A34
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @800869D8
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80086A64
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @800869C8
@80086A64
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80086AB0
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80086AB0
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80086AB0
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80086AB0
li r15, 0x4
@80086AB0
cmpwi r15, 0x3
ble @80086AD0
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086AD0
li r3, 0x0
b @80086B24
@80086AD0
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80086B04
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80086B18
@80086B04
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80086B18
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80086B24
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80086B44
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80086B44
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80086B68
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80086B7C
@80086B68
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80086B7C
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@80086B90
li r20, 0x0
@80086B94
li r18, 0x0
mr r16, r24
mr r19, r18
li r17, lbl_80478950@sda21
@80086BA4
lwz r0, 0x0(r16)
cmpwi r0, 0x5
beq @80086BB8
cmpwi r0, 0x4
bne @80086BF0
@80086BB8
addi r3, r19, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80086BF0
bl fn_80103CB0
lbz r0, 0x0(r17)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r16)
stw r0, 0x28(r24)
@80086BF0
lwz r0, 0x0(r16)
cmpwi r0, 0x7
bne @80086C00
li r18, 0x1
@80086C00
addi r16, r16, 0x4
addi r17, r17, 0x1
addi r19, r19, 0x1
cmpwi r19, 0x3
ble @80086BA4
clrlwi r0, r18, 24
cmplwi r0, 0x0
bne @80086C30
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @80086B94
@80086C30
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80086C7C
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80086C7C
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80086C7C
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80086C7C
li r16, 0x4
@80086C7C
cmpwi r16, 0x3
ble @80086C9C
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086C9C
li r3, 0x0
b @80086CF0
@80086C9C
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @80086CD0
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80086CE4
@80086CD0
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80086CE4
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80086CF0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086D1C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @80086D14
li r0, 0x1
b @80086DA8
@80086D14
li r0, 0x0
b @80086DA8
@80086D1C
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x3c47
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086D78
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @80086D98
@80086D78
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086D98
li r0, 0x1
b @80086DA8
@80086D98
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@80086DA8
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x0
b @80087AAC
@80086DBC
cmplwi r31, 0x0
beq @8008702C
lwz r0, 0x8(r21)
rlwinm r0, r0, 0, 27, 27
cmplwi r0, 0x0
bne @8008702C
li r20, 0x0
@80086DD8
li r18, 0x0
mr r16, r24
mr r19, r18
li r17, lbl_80478950@sda21
@80086DE8
lwz r0, 0x0(r16)
cmpwi r0, 0x5
beq @80086DFC
cmpwi r0, 0x4
bne @80086E34
@80086DFC
addi r3, r19, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80086E34
bl fn_80103CB0
lbz r0, 0x0(r17)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r16)
stw r0, 0x28(r24)
@80086E34
lwz r0, 0x0(r16)
cmpwi r0, 0x7
bne @80086E44
li r18, 0x1
@80086E44
addi r16, r16, 0x4
addi r17, r17, 0x1
addi r19, r19, 0x1
cmpwi r19, 0x3
ble @80086DE8
clrlwi r0, r18, 24
cmplwi r0, 0x0
bne @80086E74
bl fn_800F0308
addi r20, r20, 0x1
cmpwi r20, 0xf
blt @80086DD8
@80086E74
li r3, 0x26
bl fn_80166A28
li r16, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @80086EC0
li r16, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @80086EC0
li r16, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @80086EC0
li r16, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @80086EC0
li r16, 0x4
@80086EC0
cmpwi r16, 0x3
ble @80086EE0
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086EE0
li r3, 0x0
b @80086F34
@80086EE0
li r3, 0x1
bl fn_80103CC0
addi r4, r16, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r16, 0x0
bne @80086F14
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80086F28
@80086F14
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80086F28
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80086F34
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086F60
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
beq @80086F58
li r0, 0x1
b @80086FEC
@80086F58
li r0, 0x0
b @80086FEC
@80086F60
lbz r0, 0x21(r24)
li r4, 0x6
li r3, 0x2f
extsb r0, r0
slwi r0, r0, 2
stwx r4, r24, r0
lbz r0, 0x21(r24)
extsb r4, r0
addi r4, r4, 0x1
bl fn_80132A38
li r3, 0x7
li r4, 0x4417
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x24(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @80086FBC
mr r3, r24
li r4, 0x1
bl fn_80087AE8
b @80086FDC
@80086FBC
mr r3, r24
li r4, 0x7
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80086FDC
li r0, 0x1
b @80086FEC
@80086FDC
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r0, 0x0
@80086FEC
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80084E44
li r3, 0x2f
li r4, 0x0
bl fn_80132A38
li r3, 0x7
li r4, 0x44cf
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
mr r3, r24
li r4, 0x1
bl fn_80087AE8
li r3, 0x0
b @80087AAC
@8008702C
mr r3, r28
bl fn_80093698
li r3, 0x3cc
bl fn_80166A28
li r0, 0x5
stwx r0, r24, r27
bl fn_80103CB0
li r4, lbl_80478950@sda21
lbzx r0, r4, r28
or r0, r0, r3
clrlwi r3, r0, 24
bl fn_80103CC0
mr r4, r26
li r3, 0x2f
bl fn_80132A38
li r3, 0x7
li r4, 0x3c4b
li r5, 0x0
li r6, 0x0
bl fn_80106D3C
lwz r0, 0x8(r1)
cmpwi r0, 0x2
beq @8008749C
bge @8008709C
cmpwi r0, 0x0
beq @800870A8
bge @8008729C
b @8008789C
@8008709C
cmpwi r0, 0x4
bge @8008789C
b @8008769C
@800870A8
mr r3, r24
li r4, 0x3
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800870C8
li r3, 0x1
b @80087AAC
@800870C8
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@800870D4
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@800870E4
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @800870F8
cmpwi r0, 0x4
bne @80087130
@800870F8
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087130
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80087130
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80087140
li r16, 0x1
@80087140
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @800870E4
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80087170
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @800870D4
@80087170
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800871BC
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800871BC
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800871BC
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800871BC
li r15, 0x4
@800871BC
cmpwi r15, 0x3
ble @800871DC
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800871DC
li r3, 0x0
b @80087230
@800871DC
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80087210
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087224
@80087210
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087224
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80087230
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087250
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80087250
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80087274
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087288
@80087274
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087288
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@8008729C
extsb r0, r23
cmpwi r0, 0x2
bne @8008789C
mr r3, r24
li r4, 0x3
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800872C8
li r3, 0x1
b @80087AAC
@800872C8
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@800872D4
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@800872E4
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @800872F8
cmpwi r0, 0x4
bne @80087330
@800872F8
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087330
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80087330
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80087340
li r16, 0x1
@80087340
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @800872E4
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80087370
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @800872D4
@80087370
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800873BC
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800873BC
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800873BC
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800873BC
li r15, 0x4
@800873BC
cmpwi r15, 0x3
ble @800873DC
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800873DC
li r3, 0x0
b @80087430
@800873DC
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80087410
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087424
@80087410
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087424
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80087430
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087450
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80087450
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80087474
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087488
@80087474
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087488
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@8008749C
extsb r0, r23
cmpwi r0, 0x3
bne @8008789C
mr r3, r24
li r4, 0x3
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800874C8
li r3, 0x1
b @80087AAC
@800874C8
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@800874D4
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@800874E4
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @800874F8
cmpwi r0, 0x4
bne @80087530
@800874F8
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087530
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80087530
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80087540
li r16, 0x1
@80087540
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @800874E4
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80087570
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @800874D4
@80087570
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800875BC
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800875BC
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800875BC
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800875BC
li r15, 0x4
@800875BC
cmpwi r15, 0x3
ble @800875DC
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800875DC
li r3, 0x0
b @80087630
@800875DC
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80087610
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087624
@80087610
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087624
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80087630
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087650
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80087650
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80087674
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087688
@80087674
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087688
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@8008769C
extsb r0, r23
cmpwi r0, 0x0
bne @8008789C
mr r3, r24
li r4, 0x3
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800876C8
li r3, 0x1
b @80087AAC
@800876C8
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@800876D4
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@800876E4
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @800876F8
cmpwi r0, 0x4
bne @80087730
@800876F8
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087730
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80087730
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80087740
li r16, 0x1
@80087740
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @800876E4
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80087770
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @800876D4
@80087770
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800877BC
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800877BC
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800877BC
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800877BC
li r15, 0x4
@800877BC
cmpwi r15, 0x3
ble @800877DC
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800877DC
li r3, 0x0
b @80087830
@800877DC
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80087810
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087824
@80087810
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087824
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80087830
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087850
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80087850
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80087874
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087888
@80087874
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087888
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@8008789C
lwz r3, 0xc30(r1)
li r4, 0x7
lbz r0, 0x1(r3)
mr r3, r24
stb r0, 0x21(r24)
bl fn_80087AE8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087A94
li r3, 0x1
bl fn_80103CC0
li r15, 0x0
@800878CC
li r16, 0x0
mr r18, r24
mr r17, r16
li r19, lbl_80478950@sda21
@800878DC
lwz r0, 0x0(r18)
cmpwi r0, 0x5
beq @800878F0
cmpwi r0, 0x4
bne @80087928
@800878F0
addi r3, r17, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087928
bl fn_80103CB0
lbz r0, 0x0(r19)
andc r0, r3, r0
clrlwi r3, r0, 24
bl fn_80103CC0
li r3, 0x7
li r0, 0x8
stw r3, 0x0(r18)
stw r0, 0x28(r24)
@80087928
lwz r0, 0x0(r18)
cmpwi r0, 0x7
bne @80087938
li r16, 0x1
@80087938
addi r18, r18, 0x4
addi r19, r19, 0x1
addi r17, r17, 0x1
cmpwi r17, 0x3
ble @800878DC
clrlwi r0, r16, 24
cmplwi r0, 0x0
bne @80087968
bl fn_800F0308
addi r15, r15, 0x1
cmpwi r15, 0xf
blt @800878CC
@80087968
li r3, 0x26
bl fn_80166A28
li r15, 0x0
lwz r0, 0x0(r24)
cmpwi r0, 0x7
beq @800879B4
li r15, 0x1
lwz r0, 0x4(r24)
cmpwi r0, 0x7
beq @800879B4
li r15, 0x2
lwz r0, 0x8(r24)
cmpwi r0, 0x7
beq @800879B4
li r15, 0x3
lwz r0, 0xc(r24)
cmpwi r0, 0x7
beq @800879B4
li r15, 0x4
@800879B4
cmpwi r15, 0x3
ble @800879D4
lwz r0, 0x28(r24)
rlwinm r0, r0, 0, 28, 28
cmplwi r0, 0x0
bne @800879D4
li r3, 0x0
b @80087A28
@800879D4
li r3, 0x1
bl fn_80103CC0
addi r4, r15, 0x1
li r3, 0x2f
bl fn_80132A38
cmpwi r15, 0x0
bne @80087A08
li r3, 0x7
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087A1C
@80087A08
li r3, 0x7
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087A1C
li r0, 0x8
li r3, 0x1
stw r0, 0x28(r24)
@80087A28
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80087A48
lbz r0, 0x21(r24)
li r3, 0x6
extsb r0, r0
slwi r0, r0, 2
stwx r3, r24, r0
@80087A48
lwz r0, 0x2c(r24)
cmpwi r0, 0x3
bne @80087A6C
li r3, 0x7
li r4, 0x44e7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @80087A80
@80087A6C
li r3, 0x7
li r4, 0x44e6
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@80087A80
lbz r3, 0x21(r24)
extsb r3, r3
bl fn_80093698
li r3, 0x0
b @80087AAC
@80087A94
lwz r4, 0xc30(r1)
addi r25, r25, 0x1
addi r4, r4, 0x1
stw r4, 0xc30(r1)
cmplwi r25, 0x4
blt @80084DD8
@80087AAC
psq_l f31, -0x338(r1), 0, 0
lfd f31, 0xcc0(r1)
psq_l f30, -0x348(r1), 0, 0
lfd f30, 0xcb0(r1)
psq_l f29, -0x358(r1), 0, 0
lfd f29, 0xca0(r1)
psq_l f28, -0x368(r1), 0, 0
lfd f28, 0xc90(r1)
psq_l f27, -0x378(r1), 0, 0
lfd f27, 0xc80(r1)
lmw r15, 0xc3c(r1)
lwz r0, 0xcd4(r1)
mtlr r0
addi r1, r1, 0xcd0
blr
