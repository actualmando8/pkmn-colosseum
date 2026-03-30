    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x44
    stw r0, 0x24(r1)
    li r6, 0x0
    stmw r29, 0x14(r1)
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F7BA0
    li r3, 0x0
    b @L_801F7C40
    @L_801F7BA0:
    addi r5, r1, 0x8
    li r4, 0x1
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    mr. r31, r3
    bne @L_801F7BC8
    li r3, 0x0
    b @L_801F7C40
    @L_801F7BC8:
    li r29, 0x0
    b @L_801F7C2C
    @L_801F7BD0:
    clrlslwi r0, r29, 16, 2
    add r30, r31, r0
    mr r3, r30
    bl fn_801429E8
    clrlwi. r0, r3, 24
    beq @L_801F7C28
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r0, r3, 16
    beq @L_801F7C28
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r0, r3, 16
    beq @L_801F7C28
    li r3, 0x1
    b @L_801F7C40
    @L_801F7C28:
    addi r29, r29, 0x1
    @L_801F7C2C:
    lhz r0, 0x8(r1)
    clrlwi r3, r29, 16
    cmplw r3, r0
    blt @L_801F7BD0
    li r3, 0x0
    @L_801F7C40:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
