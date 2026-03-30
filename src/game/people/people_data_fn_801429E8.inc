    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bne @L_80142A08
    li r3, 0x0
    b @L_80142A74
    @L_80142A08:
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r31, r3, 16
    bne @L_80142A28
    li r3, 0x0
    b @L_80142A74
    @L_80142A28:
    mr r4, r31
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80142A4C
    li r0, 0x0
    b @L_80142A64
    @L_80142A4C:
    lwz r0, lbl_80478BD8(r13)
    cmplw r31, r0
    blt @L_80142A60
    li r0, 0x0
    b @L_80142A64
    @L_80142A60:
    li r0, 0x1
    @L_80142A64:
    clrlwi r3, r0, 24
    neg r0, r3
    or r0, r0, r3
    srwi r3, r0, 31
    @L_80142A74:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
