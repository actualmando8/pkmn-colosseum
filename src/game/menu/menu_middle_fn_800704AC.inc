    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r4
    lbz r0, 0x1(r3)
    lis r3, lbl_80267EA8@ha
    addi r30, r3, lbl_80267EA8@l
    extsb r0, r0
    cmpwi r0, 0x3
    bge @L_800704F0
    cmpwi r0, 0x0
    bge @L_800704F8
    b @L_80070624
    @L_800704F0:
    cmpwi r0, 0x6
    b @L_80070624
    @L_800704F8:
    bl fn_8007162C
    subi r0, r3, 0xa8
    cmplwi r0, 0x46
    bgt @L_8007061C
    lis r3, jumptable_802EE0F0@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802EE0F0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    addi r0, r30, 0x28
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    addi r0, r30, 0x40
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    addi r0, r30, 0x58
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    addi r0, r30, 0x64
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    lis r3, lbl_802EDE58@ha
    addi r30, r30, 0x78
    addi r29, r3, lbl_802EDE58@l
    li r28, 0x0
    @L_80070560:
    mr r3, r28
    bl fn_8006B3C8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8007057C
    lwz r0, 0x0(r30)
    b @L_80070580
    @L_8007057C:
    li r0, 0x43fe
    @L_80070580:
    stw r0, 0x0(r29)
    addi r30, r30, 0x4
    addi r29, r29, 0x4
    addi r28, r28, 0x1
    cmplwi r28, 0x6
    blt @L_80070560
    lis r3, lbl_802EDE58@ha
    addi r0, r3, lbl_802EDE58@l
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    li r3, 0xbc
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_800705C4
    addi r0, r30, 0x90
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    @L_800705C4:
    addi r0, r30, 0x9c
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    addi r0, r30, 0xa8
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    addi r0, r30, 0xb4
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    bl fn_8007162C
    mr r30, r3
    bl fn_801046B8
    cmpw r3, r30
    bne @L_80070604
    li r0, 0x0
    b @L_80070608
    @L_80070604:
    li r0, 0x1
    @L_80070608:
    slwi r3, r0, 2
    la r0, lbl_8047C048(r2)
    add r0, r0, r3
    stw r0, lbl_8047A5F8(r13)
    b @L_80070624
    @L_8007061C:
    li r0, 0x0
    stw r0, lbl_8047A5F8(r13)
    @L_80070624:
    lwz r0, lbl_8047A5F8(r13)
    cmplwi r0, 0x0
    beq @L_80070690
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    lha r0, 0x6(r31)
    cmpwi r0, 0x93d
    beq @L_80070654
    bge @L_800706A4
    cmpwi r0, 0x93a
    b @L_800706A4
    @L_80070654:
    bl fn_8007162C
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_8007066C
    bl fn_801046B8
    bl fn_80104704
    @L_8007066C:
    cmplwi r3, 0x0
    beq @L_800706A4
    lbz r0, 0x95(r3)
    lwz r3, lbl_8047A5F8(r13)
    extsb r0, r0
    slwi r0, r0, 2
    lwzx r0, r3, r0
    stw r0, 0x4c(r31)
    b @L_800706A4
    @L_80070690:
    li r0, 0x0
    mr r3, r31
    stw r0, 0x4c(r31)
    li r4, 0x0
    bl fn_80109220
    @L_800706A4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
