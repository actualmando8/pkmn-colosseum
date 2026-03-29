    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    cmpwi r30, 0x0
    bgt @L_800F9D58
    b @L_800F9E50
@L_800F9D58:
    mr r3, r29
    bl fn_800FE010
    addi r0, r3, 0x1
    srwi r31, r0, 1
    cmpw r31, r30
    blt @L_800F9D74
    subi r31, r30, 0x1
@L_800F9D74:
    mr r3, r28
    mr r4, r29
    slwi r5, r31, 1
    bl memcpy
    mr r6, r31
    cmpw r31, r30
    subf r3, r31, r30
    li r5, 0x0
    bge @L_800F9E4C
    srwi r0, r3, 3
    cmplwi r0, 0x0
    mtctr r0
    beq @L_800F9E34
@L_800F9DA8:
    mr r4, r6
    addi r6, r6, 0x1
    slwi r0, r4, 1
    mr r4, r6
    sthx r5, r28, r0
    slwi r0, r4, 1
    addi r6, r6, 0x1
    mr r4, r6
    sthx r5, r28, r0
    slwi r0, r4, 1
    addi r6, r6, 0x1
    mr r4, r6
    sthx r5, r28, r0
    slwi r0, r4, 1
    addi r6, r6, 0x1
    mr r4, r6
    sthx r5, r28, r0
    slwi r0, r4, 1
    addi r6, r6, 0x1
    mr r4, r6
    sthx r5, r28, r0
    slwi r0, r4, 1
    addi r6, r6, 0x1
    mr r4, r6
    sthx r5, r28, r0
    slwi r0, r4, 1
    addi r6, r6, 0x1
    mr r4, r6
    sthx r5, r28, r0
    slwi r0, r4, 1
    addi r6, r6, 0x1
    sthx r5, r28, r0
    bdnz @L_800F9DA8
    andi. r3, r3, 0x7
    beq @L_800F9E4C
@L_800F9E34:
    mtctr r3
@L_800F9E38:
    mr r4, r6
    addi r6, r6, 0x1
    slwi r0, r4, 1
    sthx r5, r28, r0
    bdnz @L_800F9E38
@L_800F9E4C:
    mr r3, r28
@L_800F9E50:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
