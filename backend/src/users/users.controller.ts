import { Body, Controller, Delete, Get, Param, Patch, Post, Put, Query } from '@nestjs/common';
import { User } from './user.entity';
import { UsersService } from './users.service';

@Controller('users')
export class UsersController {
  constructor(private usersService: UsersService) {}

  @Get()
  async getAllUsers(): Promise<Array<User>> {
    return await this.usersService.getAllUsers();
  }

  @Get(':id')
  async findUserById(@Param('id') id: number): Promise<User> {
    return await this.usersService.findUserById(id);
  }

  @Put()
  async updateUser(@Body() user: User): Promise<User> {
    return await this.usersService.updateUser(user);
  }

  @Delete(':id')
  async deleteUserById(@Param('id') id: number): Promise<void> {
    return await this.usersService.deleteUser(id);
  }

  @Post()
  async addUser(@Body() user: User): Promise<User> {
    return await this.usersService.addUser(user);
  }

  @Patch('activate/:id')
  async activateUser(@Param('id') id: number): Promise<void> {
    return await this.usersService.activateUser(id);
  }

  @Patch('deactivate/:id')
  async deactivateUser(@Param('id') id: number): Promise<void> {
    return await this.usersService.deactivateUser(id);
  }

  @Post('working/:id')
  async userWorking(@Param('id') id: string): Promise<User> {
    return await this.usersService.userWorking(id);
  }
}
