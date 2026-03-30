    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8020A4EC
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r31
    li r4, 0x0
    li r5, 0x1f
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r31
    li r4, 0x0
    li r5, 0x20
    li r6, 0x0
    li r7, -0x1
    bl fn_80142B24
    mr r3, r31
    li r4, 0x0
    li r5, 0x21
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
@L_8020A4EC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
