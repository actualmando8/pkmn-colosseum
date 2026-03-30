    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    mr. r30, r4
    stw r29, 0x1c(r1)
    addi r29, r5, 0x0
    blt @L_800B57FC
    cmpwi r30, 0x7f
    blt @L_800B5804
    @L_800B57FC:
    li r3, -0x80
    b @L_800B5870
    @L_800B5804:
    addi r4, r1, 0x14
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B5818
    b @L_800B5870
    @L_800B5818:
    lwz r3, 0x14(r1)
    bl fn_800B1E0C
    slwi r0, r30, 6
    add r30, r3, r0
    lwz r3, 0x14(r1)
    mr r4, r30
    bl fn_800B4270
    addi r31, r3, 0x0
    cmpwi r31, -0xa
    bne @L_800B584C
    mr r3, r30
    bl fn_800B4308
    mr r31, r3
    @L_800B584C:
    cmpwi r31, 0x0
    blt @L_800B5864
    addi r3, r29, 0x0
    addi r4, r30, 0x0
    li r5, 0x40
    bl memcpy
    @L_800B5864:
    lwz r3, 0x14(r1)
    mr r4, r31
    bl fn_800B02F4
    @L_800B5870:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
