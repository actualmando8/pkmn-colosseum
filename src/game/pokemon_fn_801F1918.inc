    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r5
    mr r3, r4
    bl fn_802037DC
    mr r4, r3
    mr r3, r31
    bl fn_800F9E70
    mr r3, r30
    li r4, 0x0
    li r5, 0x22
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F1970
    li r0, 0x0
    stb r0, 0x16(r31)
    b @L_801F1978
    @L_801F1970:
    li r0, 0x1
    stb r0, 0x16(r31)
    @L_801F1978:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
