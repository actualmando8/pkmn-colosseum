    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_803F7A30@ha
    li r0, 0x1
    addi r3, r3, lbl_803F7A30@l
    stb r0, 0x342(r3)
@L_8007AAC4:
    lbz r0, 0x345(r3)
    cmplwi r0, 0x0
    beq @L_8007AAC4
    lis r3, lbl_803F7A30@ha
    addi r4, r1, 0x8
    addi r3, r3, lbl_803F7A30@l
    addi r3, r3, 0x28
    bl fn_800A1E54
    bl fn_8007B0D8
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
