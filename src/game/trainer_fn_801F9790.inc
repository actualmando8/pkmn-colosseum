    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x4d
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    mr r30, r3
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F97C0
    bl fn_801F1460
    @L_801F97C0:
    li r31, 0x0
    b @L_801F97E4
    @L_801F97C8:
    mr r3, r30
    mr r6, r31
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    bl fn_80207760
    addi r31, r31, 0x1
    @L_801F97E4:
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    blt @L_801F97C8
    mr r3, r30
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801FB1C0
    bl fn_80207760
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
