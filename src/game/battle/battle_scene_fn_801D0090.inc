    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    addi r0, r3, 0xd
    cmplwi r0, 0x20
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    bgt @L_801D02AC
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -8276
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x0
    b @L_801D02F8
    li r29, 0x3c29
    b @L_801D02B0
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xd
    beq @L_801D0108
    bge @L_801D0110
    cmpwi r0, 0x8
    bge @L_801D0110
    cmpwi r0, 0x5
    bge @L_801D0108
    b @L_801D0110
@L_801D0108:
    li r29, 0x3c2c
    b @L_801D02B0
@L_801D0110:
    li r29, 0x3c04
    b @L_801D02B0
    li r29, 0x3bfb
    b @L_801D02B0
    li r29, 0x3c55
    b @L_801D02B0
    li r29, 0x4421
    b @L_801D02B0
    li r29, 0x3bfa
    li r31, 0x1
    li r30, 0x0
    b @L_801D02B0
    li r29, 0x3c33
    li r31, 0x1
    li r30, 0x0
    b @L_801D02B0
    li r29, 0x3d45
    li r31, 0x1
    li r30, 0x1
    b @L_801D02B0
    li r29, 0x3c57
    b @L_801D02B0
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xd
    beq @L_801D0190
    bge @L_801D0198
    cmpwi r0, 0x8
    bge @L_801D0198
    cmpwi r0, 0x5
    bge @L_801D0190
    b @L_801D0198
@L_801D0190:
    li r29, 0x3c2e
    b @L_801D02B0
@L_801D0198:
    li r29, 0x3c06
    b @L_801D02B0
    li r29, 0x3bfd
    b @L_801D02B0
    li r29, 0x44e5
    b @L_801D02B0
    li r29, 0x3d43
    b @L_801D02B0
    li r29, 0x44d8
    b @L_801D02B0
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xd
    beq @L_801D01E8
    bge @L_801D01F0
    cmpwi r0, 0x8
    bge @L_801D01F0
    cmpwi r0, 0x5
    bge @L_801D01E8
    b @L_801D01F0
@L_801D01E8:
    li r29, 0x44d8
    b @L_801D02B0
@L_801D01F0:
    li r29, 0x4444
    b @L_801D02B0
    li r29, 0x3c34
    b @L_801D02B0
    lwz r3, -21580(r13)
    li r29, 0x4422
    lwz r0, 0x0(r3)
    cmpwi r0, 0x2
    beq @L_801D02B0
    lwz r0, 0x5c(r3)
    li r4, 0x4422
    li r5, 0x1
    li r6, 0x1
    extsb r3, r0
    bl fn_80106D3C
    li r29, 0x44e4
    b @L_801D02B0
    li r29, 0x4423
    b @L_801D02B0
    li r29, 0x3c5d
    b @L_801D02B0
    li r29, 0x3c32
    b @L_801D02B0
    li r29, 0x3c31
    b @L_801D02B0
    li r29, 0x3c5b
    b @L_801D02B0
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x3
    bge @L_801D0288
    cmpwi r0, 0x1
    bge @L_801D0278
    b @L_801D0288
@L_801D0278:
    lwz r0, 0x60(r3)
    li r29, 0x4424
    cmpwi r0, 0x32
    beq @L_801D02B0
@L_801D0288:
    li r29, 0x3c30
    b @L_801D02B0
    li r29, 0x3c5f
    li r3, 0x2f
    li r4, 0x30
    bl fn_80132A38
    b @L_801D02B0
    li r29, 0x3c30
    b @L_801D02B0
@L_801D02AC:
    li r29, 0x3c30
@L_801D02B0:
    lwz r3, -21580(r13)
    mr r4, r29
    li r5, 0x1
    li r6, 0x1
    lwz r0, 0x5c(r3)
    extsb r3, r0
    bl fn_80106D3C
    rlwinm. r0, r31, 0, 24, 31
    beq @L_801D02F4
    mr r6, r30
    li r3, 0x0
    li r4, 0x3c
    li r5, 0xaa
    bl fn_8001E074
    extsb. r0, r3
    beq @L_801D02F4
    li r31, 0x0
@L_801D02F4:
    mr r3, r31
@L_801D02F8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
