    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    beq @L_801F8504
    clrlwi r31, r4, 24
    li r29, 0x0
    b @L_801F84F8
    @L_801F84C0:
    mr r3, r28
    clrlwi r6, r29, 24
    li r4, 0x0
    li r5, 0x4e
    bl fn_801FB1C0
    cmplwi r31, 0x1
    mr r30, r3
    bne @L_801F84EC
    bl fn_8020ECA4
    clrlwi. r0, r3, 24
    beq @L_801F84F4
    @L_801F84EC:
    mr r3, r30
    bl fn_8020EDBC
    @L_801F84F4:
    addi r29, r29, 0x1
    @L_801F84F8:
    clrlwi r0, r29, 24
    cmplwi r0, 0xc
    blt @L_801F84C0
    @L_801F8504:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
