    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lwz r29, 0x4(r4)
    cmplwi r29, 0x0
    beq @L_801BE658
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_801BE5E4
    lwz r3, 0x0(r29)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801BE620
@L_801BE5E4:
    lwz r0, lbl_8047B378(r13)
    cmplwi r0, 0x0
    beq @L_801BE5F8
    lwz r3, lbl_8047B378(r13)
    b @L_801BE600
@L_801BE5F8:
    lis r3, lbl_8036D3F0@ha
    addi r3, r3, lbl_8036D3F0@l
@L_801BE600:
    bl fn_80193828
    mr. r28, r3
    bne @L_801BE63C
    addi r3, r2, -22512
    li r4, 0x884
    addi r5, r2, -22484
    bl fn_80196E10
    b @L_801BE63C
@L_801BE620:
    bl fn_80193828
    mr. r28, r3
    bne @L_801BE63C
    addi r3, r2, -22512
    li r4, 0x1ed
    addi r5, r2, -22416
    bl fn_80196E10
@L_801BE63C:
    lwz r5, 0x0(r28)
    mr r3, r28
    mr r4, r29
    lwz r12, 0x40(r5)
    mtctr r12
    bctrl
    b @L_801BE65C
@L_801BE658:
    li r28, 0x0
@L_801BE65C:
    stw r28, 0x8(r31)
    li r0, 0x3c
    lwz r3, 0x8(r30)
    sth r3, 0x4a(r31)
    lwz r3, 0xc(r30)
    stw r3, 0x10(r31)
    stw r0, 0x14(r31)
    lfs f0, 0x10(r30)
    stfs f0, 0x18(r31)
    lfs f0, 0x14(r30)
    stfs f0, 0x1c(r31)
    lfs f0, 0x18(r30)
    stfs f0, 0x20(r31)
    lwz r3, 0x1c(r30)
    lwz r0, 0x20(r30)
    stw r3, 0x28(r31)
    stw r0, 0x2c(r31)
    lwz r0, 0x24(r30)
    stw r0, 0x30(r31)
    lwz r3, 0x28(r30)
    lwz r0, 0x2c(r30)
    stw r3, 0x34(r31)
    stw r0, 0x38(r31)
    lwz r0, 0x30(r30)
    stw r0, 0x3c(r31)
    lwz r0, 0x34(r30)
    stw r0, 0x40(r31)
    lwz r0, 0x38(r30)
    stw r0, 0x44(r31)
    lbz r0, 0x3c(r30)
    stb r0, 0x48(r31)
    lbz r0, 0x3d(r30)
    stb r0, 0x49(r31)
    lwz r0, 0x40(r30)
    stw r0, 0x4c(r31)
    lfs f0, 0x44(r30)
    stfs f0, 0x50(r31)
    lwz r0, 0x48(r30)
    stw r0, 0x54(r31)
    lwz r0, 0x4c(r30)
    stw r0, 0x58(r31)
    lwz r29, 0x50(r30)
    cmplwi r29, 0x0
    beq @L_801BE750
    li r3, 0x10
    bl fn_80193B10
    mr. r28, r3
    bne @L_801BE72C
    addi r3, r2, -22512
    li r4, 0x8a1
    addi r5, r2, -22492
    bl fn_80196E10
@L_801BE72C:
    mr r3, r28
    li r4, 0x0
    li r5, 0x10
    bl memset
    mr r4, r29
    li r5, 0x10
    mr r3, r28
    bl memcpy
    b @L_801BE754
@L_801BE750:
    li r28, 0x0
@L_801BE754:
    stw r28, 0x5c(r31)
    li r3, 0x0
    li r0, 0xff
    lwz r4, 0x54(r30)
    stw r4, 0x60(r31)
    stw r3, 0x64(r31)
    lwz r3, 0x4c(r31)
    oris r3, r3, 0x8000
    stw r3, 0x4c(r31)
    stb r0, 0x70(r31)
    lwz r29, 0x58(r30)
    cmplwi r29, 0x0
    beq @L_801BE7CC
    li r3, 0x20
    bl fn_80193B10
    mr. r28, r3
    bne @L_801BE7A8
    addi r3, r2, -22512
    li r4, 0x8cc
    addi r5, r2, -22496
    bl fn_80196E10
@L_801BE7A8:
    mr r3, r28
    li r4, 0x0
    li r5, 0x20
    bl memset
    mr r4, r29
    li r5, 0x20
    mr r3, r28
    bl memcpy
    b @L_801BE7D0
@L_801BE7CC:
    li r28, 0x0
@L_801BE7D0:
    stw r28, 0xa8(r31)
    li r3, 0x2
    bl fn_8019C6EC
    lwz r0, 0x24(r1)
    li r3, 0x0
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

