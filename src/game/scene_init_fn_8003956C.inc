    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r4
    lwz r4, lbl_8047A4A8(r13)
    li r31, 0x0
    li r3, 0x0
    addi r28, r4, 0x8
    bl fn_801347D0
    clrlwi r29, r3, 16
    li r30, 0x0
    b @L_800395C4
    @L_800395A0:
    extsh r4, r30
    li r3, 0x0
    bl fn_80134768
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800395C0
    addi r31, r31, 0x1
    @L_800395C0:
    addi r30, r30, 0x1
    @L_800395C4:
    cmpw r30, r29
    blt @L_800395A0
    addi r0, r31, 0x1
    mr r3, r27
    cmpw r28, r0
    bge @L_800395E4
    li r4, 0x1
    b @L_800395E8
    @L_800395E4:
    li r4, 0x0
    @L_800395E8:
    bl fn_80109220
    li r3, 0x0
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
