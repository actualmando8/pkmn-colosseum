    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r7, 0x0
    li r8, 0x0
    stw r0, 0x14(r1)
    li r9, 0x0
    li r10, 0x0
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r4, r5
    mr r30, r3
    mr r5, r31
    bl fn_80211170
    mr r0, r3
    mr r4, r31
    mr r3, r30
    mr r31, r0
    bl fn_8023720C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802514AC
    slwi r31, r31, 1
@L_802514AC:
    mr r3, r31
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
