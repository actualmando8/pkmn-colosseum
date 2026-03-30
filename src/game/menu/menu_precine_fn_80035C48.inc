    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lbz r0, lbl_8047A438(r13)
    cmplwi r0, 0x0
    bne @L_80035D5C
    lbz r0, lbl_8047A439(r13)
    cmplwi r0, 0x0
    beq @L_80035C78
    li r0, 0x2
    b @L_80035C7C
@L_80035C78:
    li r0, 0x1
@L_80035C7C:
    stw r0, lbl_8047A458(r13)
    bl fn_80113F48
    lis r4, 0x1117
    li r5, 0x0
    addi r4, r4, 0x1800
    li r6, 0x0
    bl fn_80176E0C
    lbz r0, lbl_8047A439(r13)
    cmplwi r0, 0x0
    beq @L_80035CAC
    li r31, 0x446
    b @L_80035CB0
@L_80035CAC:
    li r31, 0x4cd
@L_80035CB0:
    bl fn_801653C4
    cmplw r31, r3
    beq @L_80035CCC
    mr r3, r31
    li r4, 0x0
    li r5, 0x7f
    bl fn_80165A20
@L_80035CCC:
    li r4, 0x0
    li r0, 0x21
    stb r4, lbl_8047A454(r13)
    li r3, 0x3cd1
    stb r4, lbl_8047A449(r13)
    stw r0, lbl_8047A444(r13)
    bl fn_800FA280
    lis r5, lbl_803A3288@ha
    mr r4, r3
    addi r3, r5, lbl_803A3288@l
    li r5, 0x50
    bl fn_800F9D24
    lis r3, lbl_803A3288@ha
    li r0, 0x0
    addi r4, r3, lbl_803A3288@l
    li r3, 0x0
    sth r0, 0x9e(r4)
    li r4, 0xd
    bl fn_80129280
    stw r3, lbl_8047A430(r13)
    lbz r0, lbl_8047A440(r13)
    lwz r3, 0x49c8(r3)
    cmplwi r0, 0x0
    stw r3, lbl_8047A44C(r13)
    beq @L_80035D5C
    li r3, 0x0
    bl fn_80083BF8
    cmpwi r3, 0x0
    ble @L_80035D54
    li r31, 0x0
    li r3, 0x0
    li r4, 0x0
    bl fn_80083AF4
    sth r31, 0x0(r3)
@L_80035D54:
    li r0, 0x0
    stb r0, lbl_8047A440(r13)
@L_80035D5C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
