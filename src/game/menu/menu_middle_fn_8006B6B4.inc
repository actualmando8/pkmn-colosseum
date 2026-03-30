    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r0, 0x0
    lis r4, 0x1
    stb r0, lbl_8047A5E0(r13)
    subi r5, r4, 0x33d4
    li r4, 0x0
    bl memset
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x1c(r31)
    bl fn_80077E50
    addis r4, r31, 0x1
    li r0, 0xa
    subi r5, r4, 0x3628
    subi r4, r3, 0x4
    mtctr r0
    @L_8006B704:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8006B704
    lwz r0, 0x4(r4)
    li r3, 0x1
    stw r0, 0x4(r5)
    bl fn_80077E50
    addis r4, r31, 0x1
    li r0, 0xa
    subi r5, r4, 0x35d4
    subi r4, r3, 0x4
    mtctr r0
    @L_8006B73C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8006B73C
    lwz r0, 0x4(r4)
    li r3, 0x2
    stw r0, 0x4(r5)
    bl fn_80077E50
    addis r4, r31, 0x1
    li r0, 0xa
    subi r5, r4, 0x3580
    subi r4, r3, 0x4
    mtctr r0
    @L_8006B774:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8006B774
    lwz r0, 0x4(r4)
    li r3, 0x0
    stw r0, 0x4(r5)
    bl fn_80077E50
    addis r4, r31, 0x1
    li r0, 0xa
    subi r5, r4, 0x352c
    subi r4, r3, 0x4
    mtctr r0
    @L_8006B7AC:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8006B7AC
    lwz r0, 0x4(r4)
    li r3, 0x0
    stw r0, 0x4(r5)
    bl fn_80077E50
    addis r4, r31, 0x1
    li r0, 0xa
    subi r5, r4, 0x34d8
    subi r4, r3, 0x4
    mtctr r0
    @L_8006B7E4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8006B7E4
    lwz r0, 0x4(r4)
    li r3, 0x0
    stw r0, 0x4(r5)
    bl fn_80077E50
    addis r4, r31, 0x1
    li r0, 0xa
    subi r6, r4, 0x3484
    subi r4, r3, 0x4
    mtctr r0
    @L_8006B81C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_8006B81C
    lwz r0, 0x4(r4)
    addis r4, r31, 0x1
    li r5, 0x6
    li r3, 0x1
    stw r0, 0x4(r6)
    li r0, 0x0
    sth r5, -0x347a(r4)
    sth r5, -0x34ce(r4)
    sth r5, -0x3522(r4)
    stb r3, -0x342c(r4)
    stb r3, -0x342b(r4)
    stb r3, -0x342a(r4)
    stb r0, -0x3429(r4)
    stb r3, -0x3428(r4)
    stb r0, -0x3427(r4)
    stb r0, -0x3425(r4)
    addi r5, r31, 0x2
    addis r3, r5, 0x1
    stb r0, -0x3424(r4)
    addi r5, r5, 0x2
    stb r0, -0x3425(r3)
    stb r0, -0x3424(r3)
    addis r3, r5, 0x1
    addi r5, r5, 0x2
    stb r0, -0x3425(r3)
    stb r0, -0x3424(r3)
    addis r3, r5, 0x1
    addi r5, r5, 0x2
    stb r0, -0x3425(r3)
    stb r0, -0x3424(r3)
    addis r3, r5, 0x1
    addi r5, r5, 0x2
    stb r0, -0x3425(r3)
    stb r0, -0x3424(r3)
    addis r3, r5, 0x1
    addi r5, r5, 0x2
    stb r0, -0x3425(r3)
    stb r0, -0x3424(r3)
    addis r3, r5, 0x1
    stb r0, -0x3425(r3)
    stb r0, -0x3424(r3)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
