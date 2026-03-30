    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    bne @L_80134FBC
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    mr r31, r3
    @L_80134FBC:
    li r30, 0x0
    mr r29, r31
    @L_80134FC4:
    addi r4, r30, 0x1
    li r3, 0x34
    bl fn_80132A38
    mr r3, r29
    li r4, 0x9
    li r5, 0x32c9
    bl fn_800F96E4
    addi r3, r29, 0x14
    li r4, 0x1e
    bl fn_801249F8
    addi r30, r30, 0x1
    addi r29, r29, 0x24a4
    cmpwi r30, 0x3
    blt @L_80134FC4
    addi r3, r31, 0x6dec
    li r4, 0xeb
    bl fn_80142A88
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
