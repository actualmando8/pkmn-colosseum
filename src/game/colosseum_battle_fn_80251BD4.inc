    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r7, 0x0
    li r8, 0x0
    stw r0, 0x24(r1)
    mr r0, r4
    li r9, 0x0
    li r10, 0x0
    stmw r28, 0x10(r1)
    mr r29, r5
    mr r28, r3
    mr r30, r6
    mr r4, r29
    mr r5, r0
    bl fn_80211170
    mr r31, r3
    mr r3, r28
    mr r4, r30
    li r5, 0x21
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80251C40
    clrlwi r0, r29, 16
    cmplwi r0, 0x39
    bne @L_80251C40
    slwi r31, r31, 1
@L_80251C40:
    mr r3, r31
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
