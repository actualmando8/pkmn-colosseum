    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_800D2584
    cmplwi r3, 0x0
    bne @L_800D7C20
    li r3, 0x0
    b @L_800D7C60
    @L_800D7C20:
    cmpwi r31, 0x1
    beq @L_800D7C4C
    bge @L_800D7C38
    cmpwi r31, 0x0
    bge @L_800D7C44
    b @L_800D7C5C
    @L_800D7C38:
    cmpwi r31, 0x3
    bge @L_800D7C5C
    b @L_800D7C54
    @L_800D7C44:
    bl fn_800D1D00
    b @L_800D7C60
    @L_800D7C4C:
    bl fn_800D1B3C
    b @L_800D7C60
    @L_800D7C54:
    bl fn_800D1A70
    b @L_800D7C60
    @L_800D7C5C:
    li r3, 0x0
    @L_800D7C60:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
