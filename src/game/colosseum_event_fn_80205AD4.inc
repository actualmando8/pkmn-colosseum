    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    beq @L_80205B18
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    mr r4, r31
    bl fn_80123368
@L_80205B18:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
