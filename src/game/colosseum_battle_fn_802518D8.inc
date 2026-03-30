    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r7, 0x0
    li r8, 0x0
    stw r0, 0x14(r1)
    mr r0, r4
    mr r4, r5
    li r9, 0x0
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r6
    mr r5, r0
    li r10, 0x0
    bl fn_80211170
    mr r0, r3
    mr r4, r31
    mr r3, r30
    li r5, 0x1f
    mr r31, r0
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80251938
    slwi r31, r31, 1
@L_80251938:
    mr r3, r31
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
