    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r7, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r6
    bl fn_8025C264
    mr r31, r3
    mr r3, r29
    mr r4, r30
    li r5, 0x30
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802551EC
    li r3, 0x0
    b @L_80255204
@L_802551EC:
    cmpwi r31, 0x0
    bne @L_802551FC
    li r3, 0x0
    b @L_80255204
@L_802551FC:
    cmpwi r31, -0x1
    li r3, 0x1
@L_80255204:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
