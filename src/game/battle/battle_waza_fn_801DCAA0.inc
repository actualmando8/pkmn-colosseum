    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r5
    mr r31, r30
    stw r29, 0x14(r1)
    mr r29, r4
    lwz r0, 0xc(r30)
    cmpwi r0, 0x3
    beq @L_801DCADC
    bge @L_801DCAE4
    b @L_801DCAE4
    b @L_801DCAE4
@L_801DCADC:
    li r0, 0x4
    b @L_801DCAE8
@L_801DCAE4:
    li r0, 0x0
@L_801DCAE8:
    lwz r4, 0x18(r29)
    add r30, r0, r30
    addi r30, r30, 0x10
    cmpwi r4, 0x0
    beq @L_801DCB34
    lwz r3, 0x24(r3)
    b @L_801DCB14
@L_801DCB04:
    lwz r0, 0x0(r3)
    cmpw r0, r4
    beq @L_801DCB1C
    lwz r3, 0xa8(r3)
@L_801DCB14:
    cmplwi r3, 0x0
    bne @L_801DCB04
@L_801DCB1C:
    lwz r3, 0x88(r3)
    li r0, 0x0
    stw r3, 0x88(r29)
    stw r0, 0x78(r29)
    stw r0, 0x7c(r29)
    b @L_801DCB90
@L_801DCB34:
    li r0, 0x4e20
    stw r0, 0x78(r29)
    bl fn_801DB060
    stw r3, 0x7c(r29)
    mr r3, r30
    li r5, 0x4e20
    lwz r4, 0x8(r31)
    lwz r6, 0x7c(r29)
    bl fn_80101244
    lwz r5, 0x8(r31)
    li r3, 0x4e20
    lwz r4, 0x7c(r29)
    addi r0, r5, 0x1f
    rlwinm r0, r0, 0, 0, 26
    add r30, r30, r0
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801DCB88
    bl fn_801195AC
    stw r3, 0x88(r29)
    b @L_801DCB90
@L_801DCB88:
    li r0, 0x0
    stw r0, 0x88(r29)
@L_801DCB90:
    lwz r4, 0x0(r31)
    li r0, 0x0
    mr r3, r30
    stw r4, 0x80(r29)
    lwz r4, 0x4(r31)
    stw r4, 0x84(r29)
    stw r0, 0x8c(r29)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
