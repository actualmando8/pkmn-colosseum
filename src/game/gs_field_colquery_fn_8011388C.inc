    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    cmplwi r29, 0x0
    beq @L_80113910
    bl fn_800FF56C
    lis r4, lbl_80408378@ha
    cmpwi r31, 0x1
    addi r4, r4, lbl_80408378@l
    li r0, 0x1
    stw r3, 0x0(r4)
    stw r29, 0x4(r4)
    stb r0, 0x8(r4)
    stw r30, 0xc(r4)
    beq @L_801138F8
    bge @L_801138F8
    cmpwi r31, 0x0
    bge @L_801138F0
    b @L_801138F8
@L_801138F0:
    li r3, 0x0
    b @L_801138FC
@L_801138F8:
    li r3, 0x3f9
@L_801138FC:
    cmplwi r3, 0x0
    beq @L_80113908
    bl fn_80166A28
@L_80113908:
    mr r3, r29
    bl fn_800FF58C
@L_80113910:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
