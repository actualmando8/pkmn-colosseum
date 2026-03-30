    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xfe
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_802077C0
    bl fn_801F1460
    mr r3, r31
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    bl fn_80209D90
    mr r3, r31
    li r4, 0x0
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    bl fn_8020A478
@L_802077C0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
