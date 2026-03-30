    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x488(r4)
    cmpwi r0, 0x7
    bne @L_800DB780
    clrlslwi r31, r31, 17, 1
    @L_800DB780:
    lwz r6, 0x480(r4)
    mr r3, r31
    lwz r4, 0x488(r4)
    addi r5, r6, 0x10
    addi r6, r6, 0x14
    bl fn_800D6A80
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x488(r4)
    cmplwi r0, 0x7
    bgt @L_800DB83C
    lis r3, jumptable_80315364@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_80315364@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lwz r3, 0x484(r4)
    li r0, 0xb8
    stb r0, 0x0(r3)
    b @L_800DB83C
    lwz r3, 0x484(r4)
    li r0, 0xa8
    stb r0, 0x0(r3)
    b @L_800DB83C
    lwz r3, 0x484(r4)
    li r0, 0xb0
    stb r0, 0x0(r3)
    b @L_800DB83C
    lwz r3, 0x484(r4)
    li r0, 0x90
    stb r0, 0x0(r3)
    b @L_800DB83C
    lwz r3, 0x484(r4)
    li r0, 0x98
    stb r0, 0x0(r3)
    b @L_800DB83C
    lwz r3, 0x484(r4)
    li r0, 0xa0
    stb r0, 0x0(r3)
    b @L_800DB83C
    lwz r3, 0x484(r4)
    li r0, 0x80
    stb r0, 0x0(r3)
    b @L_800DB83C
    lwz r3, 0x484(r4)
    li r0, 0x80
    stb r0, 0x0(r3)
    @L_800DB83C:
    lwz r3, lbl_8047AA80(r13)
    lwz r5, 0x484(r3)
    addi r0, r5, 0x1
    stw r0, 0x484(r3)
    lwz r3, 0x480(r3)
    lbz r4, 0x0(r5)
    lwz r3, 0xc(r3)
    lwz r0, 0x4(r3)
    or r0, r4, r0
    stb r0, 0x0(r5)
    lwz r3, lbl_8047AA80(r13)
    lwz r4, 0x484(r3)
    sth r31, 0x0(r4)
    addi r4, r4, 0x2
    lwz r3, lbl_8047AA80(r13)
    stw r4, 0x484(r3)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
