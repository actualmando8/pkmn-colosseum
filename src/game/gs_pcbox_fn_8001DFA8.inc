    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lha r3, 0x6(r31)
    bl fn_8005D8B8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001DFE4
    li r0, 0xff
    stb r0, 0x66(r31)
    stb r0, 0x65(r31)
    stb r0, 0x64(r31)
    b @L_8001DFF4
@L_8001DFE4:
    li r0, 0x80
    stb r0, 0x66(r31)
    stb r0, 0x65(r31)
    stb r0, 0x64(r31)
@L_8001DFF4:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
