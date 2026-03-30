    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r5, lbl_8047AA80(r13)
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800D7DDC
    mr r6, r30
    clrlwi r5, r29, 24
    li r3, 0x43
    li r4, 0x11
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D7E40
    @L_800D7DDC:
    clrlwi r31, r29, 24
    cmplwi r31, 0x9
    ble @L_800D7DFC
    lis r3, lbl_80270440@ha
    addi r3, r3, lbl_80270440@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800D7E40
    @L_800D7DFC:
    lis r3, lbl_80314610@ha
    clrlslwi r0, r29, 24, 2
    addi r4, r3, lbl_80314610@l
    mr r3, r30
    lwzx r4, r4, r0
    bl fn_800BD4B4
    mulli r5, r31, 0x30
    lis r3, lbl_80400948@ha
    mr r4, r30
    addi r0, r3, lbl_80400948@l
    add r3, r0, r5
    bl fn_800E0628
    mr r0, r31
    cmplwi r0, 0x9
    bne @L_800D7E40
    li r0, 0x1
    stb r0, lbl_8047AAC8(r13)
    @L_800D7E40:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
