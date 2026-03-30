    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r3
    lis r3, lbl_80408378@ha
    addi r4, r3, lbl_80408378@l
    addi r3, r4, 0x10
    addi r4, r4, 0x1c
    bl fn_8012FCD4
    bl fn_800FF548
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80113C7C
    mr r3, r29
    bl fn_80113248
    li r31, 0x0
    mr r30, r31
    b @L_80113C64
@L_80113BD8:
    lwz r0, lbl_80478ECC(r13)
    lwz r3, 0xc(r29)
    add r4, r0, r30
    lhz r0, 0xc(r4)
    cmplw r0, r3
    bne @L_80113C5C
    lbz r0, 0x7(r4)
    cmpwi r0, 0x2
    beq @L_80113C50
    bge @L_80113C0C
    cmpwi r0, 0x1
    bge @L_80113C14
    b @L_80113C5C
@L_80113C0C:
    cmpwi r0, 0x4
    bge @L_80113C5C
@L_80113C14:
    lhz r3, 0xe(r4)
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80113C3C
    lwz r3, 0xc(r29)
    mr r4, r31
    li r5, 0x1
    bl fn_80116958
    b @L_80113C5C
@L_80113C3C:
    lwz r3, 0xc(r29)
    mr r4, r31
    li r5, 0x3
    bl fn_80116958
    b @L_80113C5C
@L_80113C50:
    mr r4, r31
    li r5, 0x3
    bl fn_80116958
@L_80113C5C:
    addi r30, r30, 0x18
    addi r31, r31, 0x1
@L_80113C64:
    lwz r3, lbl_80478EC8(r13)
    lwz r0, 0x0(r3)
    cmplw r31, r0
    blt @L_80113BD8
    mr r3, r29
    bl fn_80112FEC
@L_80113C7C:
    mr r3, r29
    bl fn_80117164
    bl fn_800FF548
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80113CD0
    lis r4, lbl_80272088@ha
    li r3, 0x0
    addi r6, r4, lbl_80272088@l
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    lwz r0, 0x8(r6)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_80177A44
    li r3, 0x0
    li r4, 0x64
    bl fn_801779CC
    addi r3, r1, 0x8
    bl fn_80177984
@L_80113CD0:
    bl fn_80177A64
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    bl fn_800F7D38
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    bl fn_800F7C8C
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

