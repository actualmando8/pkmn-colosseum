    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r4, r29
    addi r5, r1, 0x8
    li r3, 0x0
    bl fn_801F2020
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010DA4
    mr r3, r29
    bl fn_802037DC
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r3, 0x1
    li r4, 0x76fb
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80010D70
    @L_80010D04:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80010D5C
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010D44
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010D44
    li r0, 0x1
    b @L_80010D60
    @L_80010D44:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_80010D5C
    li r0, 0x1
    b @L_80010D60
    @L_80010D5C:
    li r0, 0x0
    @L_80010D60:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    bne @L_80010D94
    bl fn_800F0308
    @L_80010D70:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 23
    cmplwi r0, 0x0
    beq @L_80010D04
    @L_80010D94:
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800111A8
    @L_80010DA4:
    cmplwi r0, 0x2
    bne @L_80010EAC
    lwz r3, 0x8(r1)
    bl fn_80207BF4
    clrlwi r7, r3, 16
    li r3, 0x0
    li r4, 0x0
    li r5, 0x57
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, 0x8(r1)
    bl fn_802037DC
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r29
    bl fn_802037DC
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    li r3, 0x1
    li r4, 0x761f
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80010E78
    @L_80010E0C:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80010E64
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010E4C
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010E4C
    li r0, 0x1
    b @L_80010E68
    @L_80010E4C:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_80010E64
    li r0, 0x1
    b @L_80010E68
    @L_80010E64:
    li r0, 0x0
    @L_80010E68:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    bne @L_80010E9C
    bl fn_800F0308
    @L_80010E78:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 23
    cmplwi r0, 0x0
    beq @L_80010E0C
    @L_80010E9C:
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800111A8
    @L_80010EAC:
    cmplwi r30, 0x0
    bne @L_80010EC0
    li r3, 0x0
    bl fn_801F2A7C
    mr r30, r3
    @L_80010EC0:
    cmplwi r30, 0x0
    bne @L_80010ED0
    li r3, 0x0
    b @L_800111A8
    @L_80010ED0:
    mr r3, r30
    clrlwi r4, r31, 16
    bl fn_801F986C
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80010EF0
    li r3, 0x0
    b @L_800111A8
    @L_80010EF0:
    mr r3, r30
    mr r4, r31
    bl fn_801F8C00
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010FD4
    mr r3, r31
    bl fn_80203848
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r3, 0x1
    li r4, 0x76fe
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80010FA0
    @L_80010F34:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80010F8C
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010F74
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80010F74
    li r0, 0x1
    b @L_80010F90
    @L_80010F74:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_80010F8C
    li r0, 0x1
    b @L_80010F90
    @L_80010F8C:
    li r0, 0x0
    @L_80010F90:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    bne @L_80010FC4
    bl fn_800F0308
    @L_80010FA0:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 23
    cmplwi r0, 0x0
    beq @L_80010F34
    @L_80010FC4:
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800111A8
    @L_80010FD4:
    cmplwi r0, 0x2
    bne @L_800110A8
    mr r3, r31
    bl fn_80203848
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r3, 0x1
    li r4, 0x76fc
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80011074
    @L_80011008:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011060
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80011048
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80011048
    li r0, 0x1
    b @L_80011064
    @L_80011048:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_80011060
    li r0, 0x1
    b @L_80011064
    @L_80011060:
    li r0, 0x0
    @L_80011064:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    bne @L_80011098
    bl fn_800F0308
    @L_80011074:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 23
    cmplwi r0, 0x0
    beq @L_80011008
    @L_80011098:
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800111A8
    @L_800110A8:
    cmplwi r0, 0x3
    bne @L_8001117C
    mr r3, r31
    bl fn_80203848
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r3, 0x1
    li r4, 0x76fd
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80011148
    @L_800110DC:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011134
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001111C
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001111C
    li r0, 0x1
    b @L_80011138
    @L_8001111C:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_80011134
    li r0, 0x1
    b @L_80011138
    @L_80011134:
    li r0, 0x0
    @L_80011138:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    bne @L_8001116C
    bl fn_800F0308
    @L_80011148:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 23
    cmplwi r0, 0x0
    beq @L_800110DC
    @L_8001116C:
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800111A8
    @L_8001117C:
    cmplwi r0, 0x0
    bne @L_8001118C
    li r3, 0x1
    b @L_800111A8
    @L_8001118C:
    lis r3, lbl_80266788@ha
    lis r4, lbl_802E4B78@ha
    addi r3, r3, lbl_80266788@l
    addi r4, r4, lbl_802E4B78@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_800111A8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
