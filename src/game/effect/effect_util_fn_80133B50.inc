    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr. r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    beq @L_80133B7C
    li r0, 0x0
    stw r0, 0x0(r30)
    @L_80133B7C:
    li r31, 0x0
    @L_80133B80:
    mr r3, r29
    mr r4, r31
    bl fn_80133E1C
    cmplwi r30, 0x0
    beq @L_80133BAC
    bl fn_800FA444
    lwz r0, 0x0(r30)
    srwi r3, r3, 16
    cmpw r0, r3
    bge @L_80133BAC
    stw r3, 0x0(r30)
    @L_80133BAC:
    mr r4, r31
    mr r3, r29
    addi r31, r31, 0x1
    bl fn_80133BE4
    cmpwi r3, 0x0
    beq @L_80133B80
    lwz r0, 0x24(r1)
    mr r3, r31
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
