    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r29, r5
    mr r27, r3
    mr r30, r6
    li r4, 0x0
    li r5, 0x1
    bl fn_80235B04
    lis r4, 0x8025
    mr r31, r3
    addi r9, r4, 3004
    mr r3, r27
    mr r4, r29
    mr r5, r28
    mr r6, r30
    li r7, 0x0
    li r8, 0x0
    li r10, 0x0
    bl fn_80211170
    clrlwi. r0, r31, 24
    beq @L_80250BA8
    slwi r3, r3, 1
@L_80250BA8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
