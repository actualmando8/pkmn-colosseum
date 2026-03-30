    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stmw r27, 0x24(r1)
    addi r28, r6, 0x0
    addi r27, r5, 0x0
    addi r31, r4, 0x0
    addi r30, r3, 0x0
    addi r29, r7, 0x0
    addi r4, r27, 0x0
    addi r5, r28, 0x0
    addi r6, r1, 0x1c
    bl fn_800B4994
    cmpwi r3, 0x0
    bge @L_800B50B0
    b @L_800B5170
    @L_800B50B0:
    lwz r3, 0x1c(r1)
    lwz r4, 0xc(r3)
    subi r4, r4, 0x1
    and. r0, r28, r4
    bne @L_800B50CC
    and. r0, r27, r4
    beq @L_800B50D8
    @L_800B50CC:
    li r4, -0x80
    bl fn_800B02F4
    b @L_800B5170
    @L_800B50D8:
    bl fn_800B1E0C
    lwz r0, 0x4(r30)
    slwi r0, r0, 6
    add r4, r3, r0
    lwz r3, 0x1c(r1)
    bl fn_800B4270
    mr. r4, r3
    bge @L_800B5104
    lwz r3, 0x1c(r1)
    bl fn_800B02F4
    b @L_800B5170
    @L_800B5104:
    addi r3, r31, 0x0
    addi r4, r27, 0x0
    bl fn_8009B300
    cmplwi r29, 0x0
    beq @L_800B5120
    mr r0, r29
    b @L_800B5128
    @L_800B5120:
    lis r3, fn_800AF280@ha
    addi r0, r3, fn_800AF280@l
    @L_800B5128:
    lwz r4, 0x1c(r1)
    lis r3, fn_800B4FC0@ha
    addi r5, r3, fn_800B4FC0@l
    stw r0, 0xd0(r4)
    lwz r3, 0x1c(r1)
    stw r31, 0xb4(r3)
    lwz r3, 0x1c(r1)
    lhz r0, 0x10(r30)
    lwz r4, 0xc(r3)
    lwz r3, 0x0(r30)
    mullw r4, r4, r0
    bl fn_800AFFE0
    mr. r30, r3
    bge @L_800B516C
    lwz r3, 0x1c(r1)
    mr r4, r30
    bl fn_800B02F4
    @L_800B516C:
    mr r3, r30
    @L_800B5170:
    lmw r27, 0x24(r1)
    lwz r0, 0x3c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
