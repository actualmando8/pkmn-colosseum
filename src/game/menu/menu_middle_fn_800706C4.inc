    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    lbz r0, 0x1(r29)
    extsb r0, r0
    cmpwi r0, 0x3
    bge @L_8007088C
    cmpwi r0, 0x0
    bge @L_80070700
    b @L_8007088C
    @L_80070700:
    lwz r0, 0x4c(r30)
    li r4, 0x0
    stw r0, lbl_8047A5F4(r13)
    bl fn_801040D0
    subi r0, r3, 0xa8
    cmplwi r0, 0x43
    bgt @L_80070880
    lis r3, jumptable_802EE20C@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802EE20C@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x3be5
    li r0, 0x1
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3c2a
    li r0, 0x1
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3c1c
    li r0, 0x1
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3c52
    li r0, 0x1
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3d87
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3c18
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3c1a
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3c5a
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x4402
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3d32
    li r0, 0x1
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x3bfc
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x44c7
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    bl fn_8006AFC4
    cmplwi r3, 0x0
    beq @L_80070880
    bl fn_8006B420
    mr r31, r3
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    bl fn_8006AFC4
    mr r4, r31
    addi r3, r3, 0xb44
    bl fn_800767B8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80070880
    li r3, 0x3bfc
    li r0, 0x2
    stw r3, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    b @L_8007088C
    @L_80070880:
    li r0, 0x0
    stw r0, lbl_8047A5F4(r13)
    stw r0, lbl_8047A5F0(r13)
    @L_8007088C:
    lwz r3, lbl_8047A5F4(r13)
    bl fn_800FA444
    lha r0, 0x6(r30)
    srwi r3, r3, 16
    subi r3, r3, 0x20
    cmpwi r0, 0x939
    extsh r31, r3
    beq @L_800709C8
    bge @L_800708D8
    cmpwi r0, 0x809
    beq @L_80070974
    bge @L_800708C8
    cmpwi r0, 0x808
    bge @L_80070938
    b @L_80070A80
    @L_800708C8:
    cmpwi r0, 0x937
    beq @L_80070984
    bge @L_800709A4
    b @L_80070A80
    @L_800708D8:
    cmpwi r0, 0xef8
    beq @L_80070A48
    bge @L_800708F4
    cmpwi r0, 0xef6
    beq @L_80070A04
    bge @L_80070A24
    b @L_80070A80
    @L_800708F4:
    cmpwi r0, 0xefa
    bge @L_80070A80
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x2
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    lwz r0, lbl_8047A5F0(r13)
    cmpwi r0, 0x2
    bne @L_8007092C
    lwz r0, lbl_8047A5F4(r13)
    b @L_80070930
    @L_8007092C:
    li r0, 0x0
    @L_80070930:
    stw r0, 0x4c(r30)
    b @L_80070A80
    @L_80070938:
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x1
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    lwz r0, lbl_8047A5F0(r13)
    cmpwi r0, 0x1
    bne @L_80070968
    lwz r0, lbl_8047A5F4(r13)
    b @L_8007096C
    @L_80070968:
    li r0, 0x0
    @L_8007096C:
    stw r0, 0x4c(r30)
    b @L_80070A80
    @L_80070974:
    mr r3, r30
    li r4, 0x0
    bl fn_80109220
    b @L_80070A80
    @L_80070984:
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x2
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    b @L_80070A80
    @L_800709A4:
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x2
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    sth r31, 0x54(r30)
    b @L_80070A80
    @L_800709C8:
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x2
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x938
    bl fn_801046C8
    lha r0, 0x50(r3)
    add r0, r31, r0
    extsh r0, r0
    sth r0, 0x50(r30)
    b @L_80070A80
    @L_80070A04:
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x1
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    b @L_80070A80
    @L_80070A24:
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x1
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    sth r31, 0x54(r30)
    b @L_80070A80
    @L_80070A48:
    lwz r0, lbl_8047A5F0(r13)
    mr r3, r30
    subfic r0, r0, 0x1
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0xef7
    bl fn_801046C8
    lha r0, 0x50(r3)
    add r0, r31, r0
    extsh r0, r0
    sth r0, 0x50(r30)
    @L_80070A80:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
