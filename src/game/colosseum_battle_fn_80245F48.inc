    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    li r3, 0x0
    li r4, 0x0
    li r5, 0xf
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r3, r3, 16
    bl fn_801363E8
    mr r5, r3
    clrlwi r0, r30, 16
    clrlwi r3, r3, 16
    cmplw r3, r0
    beq @L_80245FAC
    mr r3, r28
    mr r4, r29
    mr r6, r31
    bl fn_8023CA9C
    b @L_80245FB0
@L_80245FAC:
    li r3, 0x0
@L_80245FB0:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
