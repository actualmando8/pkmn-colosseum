    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r28, r3
    mr r30, r5
    mr r4, r6
    li r31, 0x0
    bl fn_80236520
    clrlwi. r0, r3, 16
    beq @L_80240770
    cmplwi r0, 0xffff
    beq @L_80240770
    cmplwi r0, 0x165
    beq @L_80240770
    cmplwi r0, 0x163
    beq @L_80240770
    mr r4, r3
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    bne @L_802406B8
    lis r3, 0x8025
    addi r3, r3, -6868
@L_802406B8:
    lis r4, 0x8024
    addi r0, r4, 13468
    cmplw r3, r0
    beq @L_80240728
    lis r4, 0x8024
    addi r0, r4, 13200
    cmplw r3, r0
    beq @L_80240728
    lis r4, 0x8024
    addi r0, r4, 12932
    cmplw r3, r0
    beq @L_80240728
    lis r4, 0x8024
    addi r0, r4, 12664
    cmplw r3, r0
    beq @L_80240728
    lis r4, 0x8024
    addi r0, r4, 12516
    cmplw r3, r0
    beq @L_80240728
    lis r4, 0x8024
    addi r0, r4, 12268
    cmplw r3, r0
    beq @L_80240728
    lis r4, 0x8024
    addi r0, r4, 11852
    cmplw r3, r0
    bne @L_80240770
@L_80240728:
    mr r4, r28
    li r3, 0x0
    li r5, 0x1c2
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1c2
    bl fn_80239EE8
@L_80240770:
    mr r3, r31
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
