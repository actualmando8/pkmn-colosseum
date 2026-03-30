    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_80208044
    li r3, 0x0
    b @L_80208074
@L_80208044:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80208064
    li r3, 0x0
    b @L_80208074
@L_80208064:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80208074:
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x0
    li r5, 0x10
    li r6, 0x0
    bl fn_80142CF4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
