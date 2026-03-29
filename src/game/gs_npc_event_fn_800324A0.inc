    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r3, 0x0
    bl fn_80112260
    lbz r0, lbl_8047A408(r13)
    cmplwi r0, 0x0
    beq @L_800324D0
    li r0, 0x0
    stb r0, lbl_8047A408(r13)
    b @L_80032550
    @L_800324D0:
    li r3, 0x1
    li r0, -0x1
    li r4, 0x0
    stw r3, lbl_8047A42C(r13)
    stb r4, lbl_8047A409(r13)
    stw r0, lbl_8047A428(r13)
    stw r0, lbl_8047A424(r13)
    stw r0, lbl_8047A420(r13)
    stb r3, lbl_8047A410(r13)
    stb r3, lbl_8047A41C(r13)
    stb r3, lbl_8047A40A(r13)
    bl fn_80113F48
    lis r4, 0xffe
    mr r31, r3
    addi r3, r4, 0x1000
    bl fn_801CBA0C
    mr r4, r3
    mr r3, r31
    stw r4, lbl_8047A418(r13)
    bl fn_800F9318
    stw r3, lbl_8047A414(r13)
    lis r3, 0xfff
    addi r4, r3, 0x1800
    li r3, 0x37c
    li r5, 0x0
    li r6, 0x1
    bl fn_80176E0C
    li r3, 0x4
    bl fn_80177A44
    lis r3, 0x10b1
    addi r3, r3, 0x1000
    bl fn_801CB7C4
    @L_80032550:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
