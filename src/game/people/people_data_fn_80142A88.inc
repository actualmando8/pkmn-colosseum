    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr. r28, r3
    beq @L_80142B04
    clrlwi r31, r4, 16
    li r29, 0x0
    b @L_80142AF8
    @L_80142AB8:
    clrlslwi r0, r29, 16, 2
    add. r30, r28, r0
    beq @L_80142AF4
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    @L_80142AF4:
    addi r29, r29, 0x1
    @L_80142AF8:
    clrlwi r0, r29, 16
    cmplw r0, r31
    blt @L_80142AB8
    @L_80142B04:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
