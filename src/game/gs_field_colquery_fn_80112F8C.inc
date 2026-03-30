    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801157B0
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_80112FCC
    bl fn_800FF560
    mr r4, r3
    mr r8, r31
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
@L_80112FCC:
    lis r3, fn_80112F8C@ha
    addi r3, r3, fn_80112F8C@l
    bl fn_800FF0A0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
