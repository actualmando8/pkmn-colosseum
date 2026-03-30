    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r3
    mr r30, r4
    cmpwi r31, 0x0
    bne @L_8007AB40
    li r3, 0x0
    b @L_8007B074
@L_8007AB40:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    bne @L_8007ABF0
    li r3, 0x1
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_8007ABC0
    cmplwi r31, 0x10
    bgt @L_8007ABB8
    lis r3, jumptable_802EE5C0@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, jumptable_802EE5C0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x2
    b @L_8007B074
    li r3, 0x4
    b @L_8007B074
    li r3, 0x7
    b @L_8007B074
    li r3, 0x4
    b @L_8007B074
    li r3, 0xb
    b @L_8007B074
    li r3, 0x4
    b @L_8007B074
    li r3, 0x13
    b @L_8007B074
@L_8007ABB8:
    li r3, 0x0
    b @L_8007B074
@L_8007ABC0:
    li r0, 0x1
    lis r3, lbl_803F7A30@ha
    stw r0, 0x0(r30)
    lis r5, lbl_802EE508@ha
    addi r7, r5, lbl_802EE508@l
    addi r3, r3, lbl_803F7A30@l
    lbz r8, lbl_80478930(r13)
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
@L_8007ABF0:
    lis r3, lbl_803F7A30@ha
    addi r29, r3, lbl_803F7A30@l
    lbz r0, 0x345(r29)
    cmplwi r0, 0x0
    beq @L_8007B054
    addi r3, r29, 0x28
    addi r4, r1, 0x8
    bl fn_800A1E54
    li r0, 0x0
    li r3, 0x1
    stb r0, 0x345(r29)
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_8007AC34
    li r0, 0xa
    b @L_8007AC44
@L_8007AC34:
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    bne @L_8007AC44
    li r0, 0x0
@L_8007AC44:
    cmplwi r0, 0xa
    bgt @L_8007B070
    lis r3, jumptable_802EE594@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_802EE594@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    cmplwi r31, 0x10
    bgt @L_8007B070
    lis r3, jumptable_802EE550@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, jumptable_802EE550@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x2
    b @L_8007B074
    li r3, 0x4
    b @L_8007B074
    li r3, 0x7
    b @L_8007B074
    li r3, 0x4
    b @L_8007B074
    li r3, 0xb
    b @L_8007B074
    li r3, 0x4
    b @L_8007B074
    li r3, 0x13
    b @L_8007B074
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    cmpwi r31, 0xf
    bne @L_8007B070
    li r3, 0x10
    b @L_8007B074
    cmpwi r31, 0x12
    bne @L_8007AD64
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x2
    bl fn_8007B350
    cmpwi r31, 0x12
    bne @L_8007AD5C
    li r3, 0x0
    b @L_8007B074
@L_8007AD5C:
    li r3, 0x12
    b @L_8007B074
@L_8007AD64:
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    cmplwi r31, 0xc
    bgt @L_8007B070
    lis r3, jumptable_802EE51C@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, jumptable_802EE51C@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x10
    b @L_8007B074
    li r3, 0x11
    b @L_8007B074
    li r3, 0xb
    b @L_8007B074
    li r3, 0x4
    b @L_8007B074
    cmpwi r31, 0x3
    beq @L_8007ADEC
    cmpwi r31, 0xa
    beq @L_8007ADEC
    cmpwi r31, 0xc
    beq @L_8007ADEC
    cmpwi r31, 0xe
    bne @L_8007AE2C
@L_8007ADEC:
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x1
    bl fn_8007B350
    cmpwi r31, 0xe
    bne @L_8007AE24
    li r3, 0x0
    b @L_8007B074
@L_8007AE24:
    li r3, 0xe
    b @L_8007B074
@L_8007AE2C:
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    cmpwi r31, 0x6
    beq @L_8007AE80
    bge @L_8007AE6C
    cmpwi r31, 0x1
    beq @L_8007AE78
    b @L_8007B070
@L_8007AE6C:
    cmpwi r31, 0x8
    beq @L_8007AE88
    b @L_8007B070
@L_8007AE78:
    li r3, 0x2
    b @L_8007B074
@L_8007AE80:
    li r3, 0x7
    b @L_8007B074
@L_8007AE88:
    li r3, 0x4
    b @L_8007B074
    cmpwi r31, 0x1
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    lbz r8, lbl_80478930(r13)
    addi r7, r4, lbl_802EE508@l
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r0, 0x0
    beq @L_8007AF04
    cmpwi r31, 0x3
    beq @L_8007AF04
    cmpwi r31, 0x6
    beq @L_8007AF04
    cmpwi r31, 0x8
    beq @L_8007AF04
    cmpwi r31, 0xa
    beq @L_8007AF04
    cmpwi r31, 0xc
    beq @L_8007AF04
    cmpwi r31, 0xe
    beq @L_8007AF04
    cmpwi r31, 0xf
    beq @L_8007AF04
    cmpwi r31, 0x11
    beq @L_8007AF04
    cmpwi r31, 0x12
    bne @L_8007AF08
@L_8007AF04:
    li r0, 0x1
@L_8007AF08:
    cmpwi r0, 0x0
    beq @L_8007AF18
    li r9, 0x0
    b @L_8007AF1C
@L_8007AF18:
    li r9, 0x3
@L_8007AF1C:
    bl fn_8007B350
    b @L_8007B070
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    li r3, 0x14
    b @L_8007B074
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    cmpwi r31, 0x15
    beq @L_8007B070
    li r3, 0x15
    b @L_8007B074
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    cmpwi r31, 0x16
    beq @L_8007B070
    li r3, 0x16
    b @L_8007B074
    lis r3, lbl_803F7A30@ha
    lis r4, lbl_802EE508@ha
    addi r7, r4, lbl_802EE508@l
    lbz r8, lbl_80478930(r13)
    addi r3, r3, lbl_803F7A30@l
    li r4, 0x1
    la r5, lbl_8047A640(r13)
    li r6, 0x4a
    li r9, 0x3
    bl fn_8007B350
    cmpwi r31, 0x17
    beq @L_8007B070
    li r3, 0x17
    b @L_8007B074
    subi r0, r31, 0xe
    li r3, 0x0
    cmplwi r0, 0x1
    stw r3, 0x0(r30)
    ble @L_8007B020
    cmpwi r31, 0x11
    beq @L_8007B020
    cmpwi r31, 0x12
    bne @L_8007B070
@L_8007B020:
    li r3, 0x13
    b @L_8007B074
    subi r0, r31, 0xe
    li r3, 0x0
    cmplwi r0, 0x1
    stw r3, 0x0(r30)
    ble @L_8007B04C
    cmpwi r31, 0x11
    beq @L_8007B04C
    cmpwi r31, 0x12
    bne @L_8007B070
@L_8007B04C:
    li r3, 0x13
    b @L_8007B074
@L_8007B054:
    lbz r0, 0x346(r29)
    cmpwi r0, 0x2
    bne @L_8007B070
    cmpwi r31, 0xe
    bne @L_8007B070
    li r3, 0xf
    b @L_8007B074
@L_8007B070:
    li r3, 0x0
@L_8007B074:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
