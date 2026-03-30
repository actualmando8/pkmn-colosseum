    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_8000CE88
    li r3, 0x0
    b @L_8000CEBC
    @L_8000CE88:
    li r4, 0x0
    bl fn_8012AC08
    mr r31, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000CEAC
    li r3, 0x0
    b @L_8000CEBC
    @L_8000CEAC:
    mr r3, r31
    li r4, 0x25
    bl fn_80097A38
    li r3, 0x0
    @L_8000CEBC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
