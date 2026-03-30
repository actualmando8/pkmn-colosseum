    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bne @L_802035E0
    li r3, 0x0
    b @L_802035F0
@L_802035E0:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802035F0:
    cmplwi r3, 0x0
    beq @L_8020360C
    mr r7, r31
    li r4, 0x0
    li r5, 0x79
    li r6, 0x0
    bl fn_801254B4
@L_8020360C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
