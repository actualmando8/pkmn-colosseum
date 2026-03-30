    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    clrlwi. r0, r6, 23
    stwu r1, -0x38(r1)
    stmw r27, 0x24(r1)
    addi r29, r3, 0x0
    addi r30, r4, 0x0
    addi r31, r5, 0x0
    addi r27, r7, 0x0
    bne @L_800B4CAC
    clrlwi. r0, r31, 23
    beq @L_800B4CB4
    @L_800B4CAC:
    li r3, -0x80
    b @L_800B4DB0
    @L_800B4CB4:
    addi r3, r29, 0x0
    addi r4, r31, 0x0
    addi r5, r6, 0x0
    addi r6, r1, 0x1c
    bl fn_800B4994
    cmpwi r3, 0x0
    bge @L_800B4CD4
    b @L_800B4DB0
    @L_800B4CD4:
    lwz r3, 0x1c(r1)
    bl fn_800B1E0C
    lwz r0, 0x4(r29)
    slwi r0, r0, 6
    add r28, r3, r0
    lwz r3, 0x1c(r1)
    mr r4, r28
    bl fn_800B4270
    addi r4, r3, 0x0
    cmpwi r4, -0xa
    bne @L_800B4D0C
    mr r3, r28
    bl fn_800B4308
    mr r4, r3
    @L_800B4D0C:
    cmpwi r4, 0x0
    bge @L_800B4D20
    lwz r3, 0x1c(r1)
    bl fn_800B02F4
    b @L_800B4DB0
    @L_800B4D20:
    addi r3, r30, 0x0
    addi r4, r31, 0x0
    bl DCInvalidateRange
    cmplwi r27, 0x0
    beq @L_800B4D3C
    mr r0, r27
    b @L_800B4D44
    @L_800B4D3C:
    lis r3, fn_800AF280@ha
    addi r0, r3, fn_800AF280@l
    @L_800B4D44:
    lwz r3, 0x1c(r1)
    stw r0, 0xd0(r3)
    lwz r3, 0x1c(r1)
    lwz r4, 0x8(r29)
    lwz r5, 0xc(r3)
    subi r0, r5, 0x1
    and r8, r4, r0
    subf r3, r8, r5
    cmpw r31, r3
    bge @L_800B4D70
    mr r3, r31
    @L_800B4D70:
    lhz r0, 0x10(r29)
    lis r4, fn_800B4B4C@ha
    addi r31, r3, 0x0
    lwz r3, 0x0(r29)
    mullw r0, r5, r0
    addi r7, r4, fn_800B4B4C@l
    addi r5, r31, 0x0
    addi r6, r30, 0x0
    add r4, r8, r0
    bl fn_800B1864
    mr. r29, r3
    bge @L_800B4DAC
    lwz r3, 0x1c(r1)
    mr r4, r29
    bl fn_800B02F4
    @L_800B4DAC:
    mr r3, r29
    @L_800B4DB0:
    lmw r27, 0x24(r1)
    lwz r0, 0x3c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
